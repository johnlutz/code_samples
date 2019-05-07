<?php

/**
 * @file
 * Admin settings form needed for the CHTR Training REST API.
 */

/**
 * Administration form for training api configuration needed to run calls.
 */
function roms_training_api_admin_form() {
  $form = array();

  $form['description'] = array(
    '#markup' => 'The following fields are necessary to connect to the training database.',
  );

  $form['roms_training_api_company_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Company ID'),
    '#description' => t('The company ID to lookup students at the training database.'),
    '#size' => 60,
    '#required' => TRUE,
    '#default_value' => variable_get('roms_training_api_company_id', ''),
  );

  $form['roms_training_api_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Training API Access Token'),
    '#description' => t('The supplied Access Token for contacting the training database.'),
    '#size' => 60,
    '#required' => TRUE,
    '#default_value' => variable_get('roms_training_api_access_token', '62ac227dbbe1888b07afc58b0df55fcb217825920be837126c9536'),
  );

  return system_settings_form($form);
}
