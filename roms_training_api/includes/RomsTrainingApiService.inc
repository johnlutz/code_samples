<?php

/**
 * @file
 * Implementation of the CHTR Training Interface.
 */

/**
 * Class implementation of Training Interface.
 */
class RomsTrainingApiService implements RomsTrainingApiInterface {
  public $status            = '';
  public $headers           = '';
  public $json              = '';
  protected $apiAccessToken = '';
  protected $apiUrl         = 'https://training.chtr.ca/api';
  protected $apiHeaders     = array(
    'accept' => 'Accept: application/json',
  );

  /**
   * Constructor.
   */
  public function __construct() {
    $this->apiAccessToken = variable_get('roms_training_api_access_token', '62ac227dbbe1888b07afc58b0df55fcb217825920be837126c9536');
  }

  /**
   * Retrieves all the students for a particular company.
   *
   * @param int $company_id
   *   Company id we are searching students for.
   * @param array $params
   *   Extra parameters (page => 1, per-page => 20, last-mod => YYYY-MM-DD).
   *
   * @return array
   *   Returns an array of students.
   */
  public function getStudents($company_id, array $params = array()) {
    $data = array();

    $url = $this->createUrl('companies/' . $company_id . '/students', $params);
    $this->request('GET', $url);
    $data = json_decode($this->json);
    $this->objectCleanUp();

    if (!empty($data->student)) {
      if (!isset($data->student[0])) {
        $data->student = array($data->student);
      }
      return $data->student;
    }
    else {
      return array();
    }
  }

  /**
   * Retrieves all the courses for a particular student.
   *
   * @param int $student_id
   *   Student id we are searching courses for.
   * @param array $params
   *   Extra parameters (page => 1, per-page => 20, last-mod => YYYY-MM-DD).
   *
   * @return array
   *   Returns an array of students.
   */
  public function getStudentCourses($student_id, array $params = array()) {
    $data = array();

    $url = $this->createUrl('students/' . $student_id . '/courses', $params);
    $this->request('GET', $url);
    $data = json_decode($this->json);
    $this->objectCleanUp();

    if (!empty($data->course)) {
      if (!isset($data->course[0])) {
        $data->course = array($data->course);
      }
      return $data->course;
    }
    else {
      return array();
    }
  }

  /**
   * Build api url.
   *
   * @param string $path
   *   Endpoint path to service.
   * @param null|array $params
   *   Extra query string parameters to pass along.
   *
   * @return string
   *   Full url to service.
   */
  protected function createUrl($path, $params = NULL) {

    $url = $this->apiUrl . "/{$path}";

    if (!empty($params) && is_array($params)) {
      $url .= "?" . http_build_query($params);
    }

    return $url;

  }

  /**
   * Send the request to the api.
   *
   * @param string $type
   *   GET|POST|DELETE|PUT.
   * @param string $url
   *   Full url that we will send to.
   * @param array $params
   *   Fields to post.
   *
   * @return $this|false
   *   False if there was problem, the class object if ok.
   */
  protected function request($type, $url, array $params = array()) {

    // Setup curl.
    $ch = curl_init();

    curl_setopt($ch, CURLOPT_USERPWD, $this->apiAccessToken . ':');
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_setopt($ch, CURLOPT_HEADER, TRUE);
    curl_setopt($ch, CURLINFO_HEADER_OUT, TRUE);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $type);

    if ($type == 'POST') {
      if (is_array($params)) {
        $params = $this->encodeParams($params);
      }
      curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
    }

    curl_setopt($ch, CURLOPT_HTTPHEADER, $this->apiHeaders);
    curl_setopt($ch, CURLOPT_URL, $url);

    $response      = curl_exec($ch);
    $rawHeaderSize = curl_getinfo($ch, CURLINFO_HEADER_SIZE);
    $this->json    = substr($response, $rawHeaderSize);
    $this->status  = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    $this->headers = $this->parseHeaders($response, $rawHeaderSize);

    curl_close($ch);

    switch ($this->status) {
      // OK.
      case 200:
        // Saved successfully (POST, PUT) Location header contains the URL
        // pointing to the newly created resource.
      case 201:
        // Deleted successfully, no response content in body.
      case 204:
        // Not modified.
      case 304:
        return $this;

      // Bad request. This could be caused by providing invalid JSON in the
      // request body, providing invalid action parameters, etc.
      case 400:
        // Authentication failed.
      case 401:
        // The authenticated user is not allowed to access the specified API
        // endpoint.
      case 403:
        // The requested resource does not exist.
      case 404:
        // Method not allowed.
      case 405:
        // Unsupported media type. The requested content type or version number
        // is invalid.
      case 415:
        // Data validation failed (in response to a POST request, for example).
        // Please check the response body for detailed error messages.
      case 422:
        // Too many requests. The request was rejected due to rate limiting.
      case 429:
        // Internal server error. This could be caused by internal program
        // errors.
      case 500:
      default:
        $message = 'CHTR API Error<br />';
        $message .= "Request: @request<br />\n
          Params:<br />\n
          <pre>@params</pre><br />\n
          Response Code: @response_code<br />\n
          Response:<br />\n
          <pre>@response</pre>
        ";
        $args = array(
          '@request' => $url,
          '@params' => print_r($params, TRUE),
          '@response_code' => $this->status,
          '@response' => print_r($response, TRUE),
        );
        watchdog('CHTR Training API Error', $message, $args, WATCHDOG_ERROR);
        return FALSE;

    }

  }

  /**
   * Parse the headers.
   *
   * @param string $response
   *   Response from api.
   * @param int $rawHeaderSize
   *   Character length of full header.
   *
   * @return array
   *   Parsed list of headers.
   */
  protected function parseHeaders($response, $rawHeaderSize) {
    $list    = array();
    $headers = str_replace("\r", "", substr($response, 0, $rawHeaderSize));
    $headers = explode("\n", $headers);

    foreach ($headers as $header) {
      if (strstr($header, ':')) {
        $name        = strtolower(substr($header, 0, strpos($header, ':')));
        $list[$name] = trim(substr($header, strpos($header, ':') + 1));
      }
    }

    return $list;
  }

  /**
   * Clean up the class properties.
   *
   * Remove empty public properties & all protected and private properties
   * from $this for a much cleaner output.
   */
  protected function objectCleanUp() {
    $obj       = new \ReflectionObject($this);
    $public    = $obj->getProperties(\ReflectionProperty::IS_PUBLIC);
    $protected = $obj->getProperties(\ReflectionProperty::IS_PROTECTED);
    $private   = $obj->getProperties(\ReflectionProperty::IS_PRIVATE);

    // Remove any empty public properties.
    foreach ($public as $property) {
      $name = $property->getName();
      if (empty($this->$name)) {
        unset($this->$name);
      }
    }

    // Remove all protected properties.
    foreach ($protected as $property) {
      $name = $property->getName();
      unset($this->$name);
    }

    // Remove all private properties.
    foreach ($private as $property) {
      $name = $property->getName();
      unset($this->$name);
    }
  }

}
