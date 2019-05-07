<?php
/**
 * @file
 * Views integration for the ATG User Activity Tracking custom module.
 */

/**
 * Implements hook_views_data()
 */
function atg_user_activity_tracking_views_data() {
  // Adds custom activites database tables to views.
  $table = array(
    'atg_user_current_activities' => array(
      'table' => array(
        'group' => 'ATG',
        'base'  => array(
          'field'  => 'ca_id',
          'title' => t('ATG User Current Activities'),
          'help'  => t('Custom database table for tracking user current activities.')
        )
      ),
      'ca_id' => array(
        'title'  => t('Current Activity ID'),
        'help'   => t('Unique ID for the current activity stored in the custom database table.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'uid' => array(
        'title'  => t('Current Activity User ID'),
        'help'   => t('Unique ID for the user who performed the current activity.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'activity' => array(
        'title'  => t('Current Activity Description'),
        'help'   => t('Description of the current activity that was logged.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'page' => array(
        'title'  => t('Current Activity Page'),
        'help'   => t('The page on which the current action was performed.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'context' => array(
        'title'  => t('Current Activity Context'),
        'help'   => t('Entity ID that the current activity was performed on.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'timestamp' => array(
        'title'  => t('Current Activity Timestamp'),
        'help'   => t('Time the current activity was performed.'),
        'field'   => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_date'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
    ),
    'atg_user_aggregate_activities' => array(
      'table' => array(
        'group' => 'ATG',
        'base'  => array(
          'field'  => 'aa_id',
          'title' => t('ATG User Aggregate Activities'),
          'help'  => t('Custom database table for tracking user aggregate activities.')
        ),
        'join' => array(
          'users' => array(
            'left_field' => 'uid',
            'field' => 'uid',
          ),
        ),
      ),
      'aa_id' => array(
        'title'  => t('Aggregate Activity ID'),
        'help'   => t('Unique ID for the aggregate activity stored in the custom database table.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'uid' => array(
        'title'  => t('Aggregate Activity User ID'),
        'help'   => t('Unique ID for the user who performed the aggregate activity.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric'
        ),
        'filter'    => array(
          'handler' => 'views_handler_filter_numeric'
        ),
        'sort'     => array(
          'handler' => 'views_handler_sort'
        ),
        'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'handler' => 'views_handler_relationship',
          'label' => t('User'),
        ),
      ),
      'activity' => array(
        'title'  => t('Aggregate Activity Description'),
        'help'   => t('Description of the aggregate activity that was logged.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'page' => array(
        'title'  => t('Aggregate Activity Page'),
        'help'   => t('The page on which the aggregate action was performed.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'context' => array(
        'title'  => t('Aggregate Activity Context'),
        'help'   => t('Entity ID that the aggregate activity was performed on.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'start_time' => array(
        'title'  => t('Aggregate Activity Start Time'),
        'help'   => t('Time the aggregate activity action was started.'),
        'field'   => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_date'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
      'duration' => array(
        'title'  => t('Aggregate Activity Duration'),
        'help'   => t('Duration of the aggregate activity action.'),
        'field'   => array(
          'click sortable' => TRUE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_date'
        ),
        'sort'  => array(
          'handler' => 'views_handler_sort'
        ),
      ),
    )
  );

  return $table;
}