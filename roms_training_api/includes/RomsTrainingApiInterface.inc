<?php

/**
 * @file
 * Interface for the CHTR Training REST API calls.
 */

/**
 * Interface for Training API calls.
 */
interface RomsTrainingApiInterface {

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
  public function getStudents($company_id, array $params);

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
  public function getStudentCourses($student_id, array $params);

}
