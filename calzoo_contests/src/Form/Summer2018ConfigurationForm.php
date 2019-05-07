<?php

namespace Drupal\calzoo_contests\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Defines a form that configures forms module settings.
 */
class Summer2018ConfigurationForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'calzoo_contests_summer_2018_settings';
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'calzoo_contests.settings',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $form['title'] = [
      '#markup' => '<h2>Summer 2018 Contest Configuration</h2>'
    ];
    $config = $this->config('calzoo_contests.settings');
    // Get saved info.
    $codes_text = $config->get('summer_2018_campaign_codes');
    // Display usage table of codes.
    if (!empty($codes_text)) {
      $codes = explode("\r\n", $codes_text);
      $form['summer_2018_campaign_codes_table'] = [
        '#type' => 'table',
        '#caption' => $this->t('Code Usage'),
        '#header' => [
          $this->t('Code'),
          $this->t('Times Used'),
        ],
      ];
      foreach ($codes as $code) {
        // Lookup usage.
        $connection = \Drupal::service('database');
        $query = $connection->select('webform_submission_data', 'wsd');
        $query->condition('wsd.name', 'codes');
        $query->condition('wsd.value', $code);
        $query->fields('wsd', ['sid']);
        $results = $query->execute()->fetchAll();
        // Add to array to sort.
        $sorted_codes[$code] = count($results);
      }
      // Sort array by times used.
      asort($sorted_codes);
      $sorted_codes = array_reverse($sorted_codes);
      // Add to table.
      foreach ($sorted_codes as $code => $times_used) {
        $form['summer_2018_campaign_codes_table'][$code]['code'] = [
          '#markup' => $code,
        ];
        $form['summer_2018_campaign_codes_table'][$code]['usage'] = [
          '#markup' => $times_used,
        ];
      }
    }
    // Input codes.
    $form['summer_2018_campaign_codes'] = array(
      '#type' => 'textarea',
      '#title' => $this->t('Codes'),
      '#default_value' => !empty($codes_text) ? $codes_text : '',
      '#description' => $this->t('Add each code on a separate line.'),
    );

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValues();
    $this->config('calzoo_contests.settings')
      ->set('summer_2018_campaign_codes', $values['summer_2018_campaign_codes'])
      ->save();
  }

}
