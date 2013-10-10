<?php

/**
 * @file
 *   HTTP response headers drush command.
 */

/**
 * Implementation of hook_drush_command().
 */

function http_response_headers_drush_command() {
  $items = array();

  // The 'http-header-set' command
  $items['http-header-set'] = array(
    'description' => "Creates new HTTP header rule",
    'arguments' => array(
      'name' => 'The machine name of the header rule.',
      'header' => '',
      'header_value' => ''
    ),
    'options' => array(
      'path' => array(
        'description' => 'Specific pages to set HTTP headers',
        'example-value' => 'admin/*, node/123',
      ),
      'type' => array(
        'description' => 'Specific content types to set HTTP headers',
        'example-value' => 'article, page',
      ),
      'role' => array(
        'description' => 'Specific roles to set HTTP headers',
        'example-value' => 'anonymous, authenticated',
      ),
    ),
    'examples' => array(
      'drush http-header-set global_expires Expires 6000  --path=node/* --type=article --role=anonymous' => 'Make a header rule with cache set for 30 seconds on pages under node/ and type article viewed by anonymous user.',
    ),
    'aliases' => array('header-set'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

  // The 'http-header-get' command
  $items['http-header-get'] = array(
    'description' => "Retrieves HTTP header rule by machine name",
    'arguments' => array(
      'name' => 'The machine name of the header rule.',
    ),
    'examples' => array(
      'drush http-header-get global_expires' => 'Get the header rule with machine name global_expires',
    ),
    'aliases' => array('header-get'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

  // The 'http-header-get' command
  $items['http-header-del'] = array(
    'description' => "Delete a HTTP header rule by machine name",
    'arguments' => array(
      'name' => 'The machine name of the header rule.',
    ),
    'examples' => array(
      'drush http-header-del global_expires' => 'Delete the header rule with machine name global_expires',
    ),
    'aliases' => array('header-del'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

  // The 'http-header-get' command
  $items['http-header-list'] = array(
    'description' => "Lists all HTTP header rules available",
    'examples' => array(
      'drush http-header-list' => 'Delete the header rule with machine name global_expires',
    ),
    'aliases' => array('header-list'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
  );

}
