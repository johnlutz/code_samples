<?php

namespace Drupal\calzoo_contests\Element;

use Drupal\Component\Utility\NestedArray;
use Drupal\Core\Render\Element;
use Drupal\Core\Render\Element\FormElement;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Template\Attribute;
use Drupal\webform\Utility\WebformElementHelper;
use Drupal\webform\Element\WebformMultiple;

/**
 * Alters the webform mulitple element for our codes changes.
 */
class CodesWebformMultiple extends WebformMultiple {

  /**
   * Process codes.
   */
  public static function processCodes(&$element, FormStateInterface $form_state, &$complete_form) {
    // Get field name.
    $webform_key = $element['#webform_key'];

    // Run original webform mulitple process on the element.
    $element = \Drupal\webform\Element\WebformMultiple::processWebformMultiple($element, $form_state, $complete_form);

    // Making our alterations.

    // Wrap in a div for our ajax.
    $element['#prefix'] = '<div id="codes_wrapper">';
    $element['#suffix'] = '</div>';
    // Move the description to under the items.
    if (!empty($element['#description'])) {
      $element['description'] = $element['#description'];
      $element['description']['#prefix'] = '<div class="description-text"><p>';
      $element['description']['#suffix'] = '</p></div>';
      unset($element['#description']);
    }
    // Add an 'add' button.
    $element['add'] = [
      '#type' => 'submit',
      '#value' => t('+ Add Another Code'),
      '#limit_validation_errors' => [],
      '#ajax' => [
        'callback' => [get_called_class(), 'ajaxCallback'],
        'wrapper' => 'codes_wrapper',
      ],
      '#attributes' => ['class' => ['inverted']]
    ];
    // Validate the codes to see if we have any errors.
    $form_codes = $form_state->getValue($webform_key);
    if (!empty($form_codes)) {
      \Drupal\calzoo_contests\Element\CodesWebformMultiple::validateCodes($element, $form_state, $complete_form, FALSE);
    }
    $errors = $form_state->getErrors();
    $codes_error = (key_exists($webform_key, $errors));
    // Only show 1 code item at a time.
    $items = 0;
    $empty_items = [];
    foreach ($element['items'] as $key => $item) {
      if (is_numeric($key)) {
        if (!empty($form_codes[$key])) {
          $items++;
          if ($codes_error) {
            $empty_items[] = $key;
          }
        }
        else {
          $empty_items[] = $key;
          if (in_array($key-1, $empty_items)) {
            unset($element['items'][$key]);
          }
        }
      }
    }
    if ($items >= 14) {
      $element['add']['#prefix'] = '<div class="hidden">';
      $element['add']['#suffix'] = '</div>';
      $element['description']['#prefix'] = '<div class="description-text hidden"><p>';
    }
    // Remove original webform multiple validation.  We will run in ours.
    foreach ($element['#element_validate'] as $key => $function) {
      if ($function[0] == 'Drupal\webform\Element\WebformMultiple') {
        unset($element['#element_validate'][$key]);
      }
    }
    
    return $element;
  }

  /**
   * Validates multiple elements.
   */
  public static function validateCodes(array &$element, FormStateInterface $form_state, array &$form, $validate_full = TRUE) {
    // Do original validation for webform multiple.
    if ($validate_full) {
      \Drupal\webform\Element\WebformMultiple::validateWebformMultiple($element, $form_state, $complete_form);
    }
    
    // Get values.
    $form_codes = $form_state->getValue($element['#webform_key']);

    // Skip if there are no values.
    if (empty($form_codes)) {
      return;
    }

    // Check for duplicate codes.
    if (count($form_codes) !== count(array_unique($form_codes))) {
      $form_state->setError(
        $element,
        t('You have entered the same code more than once.')
      );
      return;
    }

    // Check for valid codes.
    $campaign = $element['#webform'];
    $config = \Drupal::config('calzoo_contests.settings');
    if ($valid_codes_text = $config->get($campaign . '_codes')) {
      $valid_codes = explode("\r\n", $valid_codes_text);
      // Find any invalid codes.
      foreach ($form_codes as $form_code) {
        if (!in_array($form_code, $valid_codes)) {
          $args = [
            '%form_code' => $form_code,
          ];
          $form_state->setError(
            $element,
            t('The code <em>%form_code</em> is invalid.', $args)
          );
        }
      }
    }
  }

  /**
   * Webform submission Ajax callback.
   */
  public static function ajaxCallback(array &$form, FormStateInterface $form_state) {
    $button = $form_state->getTriggeringElement();
    $element = NestedArray::getValue($form, array_slice($button['#array_parents'], 0, -1));
    return $element;
  }
}
