<?php

/**
 * @file
 * The Missing Module Message Fixer Admin Settings file.
 */

/**
 * Missing Module Message Fixer Admin Page Callback.
 */
function module_missing_message_fixer_page() {

  // Grab filtered enabled modules list.
  $table = _module_missing_message_fixer_table();

  // Grab the form processing button.
  $form = drupal_get_form('module_missing_message_fixer_form');

  $page = array(
    '#prefix' => '<center>',
    'form' => $form,
    'table' => array(
      '#markup' => $table,
    ),
    '#suffix' => '</center>',
  );

  return $page;
}

/**
 * Missing Module Message Fixer Form.
 */
function module_missing_message_fixer_form() {
  $form = array();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove These Errors!'),
    '#submit' => array('module_missing_message_fixer_form_submit'),
  );
  return $form;
}

/**
 * Submit handler for Missing Module Message Fixer Form.
 */
function module_missing_message_fixer_form_submit() {
  $modules = array();

  $query = db_query("SELECT filename, type, name FROM {system} WHERE type = :type", array(':type' => 'module'));

  foreach ($query->fetchAll() as $record) {
    // Grab the checker.
    $check = drupal_get_filename($record->type, $record->name, $record->filename, FALSE);

    // If drupal_get_filename returns null = we got problems.
    if (is_null($check) && $record->name != 'default') {
      $modules[] = $record->name;
    }
  }

  // Delete if there is no modules.
  if (count($modules) > 0) {
    db_delete('system')
      ->condition('name', $modules, 'IN')
      ->execute();
  }
}

/**
 * Generates the table for the missing module page.
 *
 * @return string
 *   The missing module formatted table output.
 */
function _module_missing_message_fixer_table() {
  $rows = array();
  $header = array('Name', 'Type', 'Filename');

  $query = db_query("SELECT filename, type, name FROM {system} WHERE type = :type", array(':type' => 'module'));

  foreach ($query->fetchAll() as $record) {
    // Grab the checker.
    $check = drupal_get_filename($record->type, $record->name, $record->filename, FALSE);

    // If drupal_get_filename returns null = we got problems.
    if (is_null($check) && $record->name != 'default') {
      $rows[] = array(
        $record->name,
        $record->type,
        $record->filename,
      );
    }
  }

  // Shows Empty Screen.
  if (!count($rows)) {
    $header = array('Empty');
    $rows[] = array(t('No Missing Modules Found!!!'));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
