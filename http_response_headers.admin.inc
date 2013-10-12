<?php

/**
 * @file
 * Admin page callbacks for the HTTP response headers module.
 */

/**
 * Page callback for headers manage page.
 */
function http_response_headers_manage() {
  $row = array();
  $build = array();
  $header = array(
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('Header'), 'field' => 'header'),
    array('data' => t('Header value')),
    array('data' => t('Operations'), 'colspan' => '2'),
  );
  $rules = http_response_headers_list($header);

  foreach ($rules as $rule) {
    $row[] = array(
      array('data' => $rule->name),
      array('data' => $rule->header),
      array('data' => check_plain($rule->header_value)),
      array('data' => l(t('configure'), "admin/config/system/http-response-headers/$rule->rule_id/configure")),
      array('data' => l(t('delete'), "admin/config/system/http-response-headers/$rule->rule_id/delete")),
    );
  }

  if ($row) {
    $pager = theme('pager');
    if (!empty($pager)) {
      $row[] = array(array('data' => $pager, 'colspan' => '3'));
    }
    $build['header_rules_title'] = array('#markup' => '<h3>' . t('Available header rules:') . '</h3>');
    $build['header_rules_table'] = array(
      '#markup' => theme('table', array(
        'header' => $header,
        'rows' => $row,
      )),
    );
  }

  return $build;
}

/**
 * Form definition for http_response_headers_add_form().
 */
function http_response_headers_add_form($form, &$form_state) {
  return http_response_headers_configure($form, $form_state, NULL);
}

/**
 * Form validation handler for http_response_headers_add_form().
 *
 * @see http_response_headers_add_form()
 * @see http_response_headers_add_form_submit()
 */
function http_response_headers_add_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];
  if (empty($form_values['pages'])) {

  }
}

/**
 * Form submission handler for http_response_headers_add_form().
 *
 * Saves the new HTTP header rule.
 */
function http_response_headers_add_form_submit($form, &$form_state) {
  $fields = array(
    'rule_id' => trim(check_plain($form_state['values']['rule_id'])),
    'name' => trim(check_plain($form_state['values']['name'])),
    'header' => trim($form_state['values']['header']),
    'header_value' => trim($form_state['values']['header_value']),
    'visibility' => (int) $form_state['values']['visibility'],
    'pages' => trim($form_state['values']['pages']),
    'types' => implode(',', array_filter($form_state['values']['types'])),
    'roles' => implode(',', array_filter($form_state['values']['roles'])),
  );
  http_response_header_add($fields);

  drupal_set_message(t('The header rule has been created.'));
  $form_state['redirect'] = 'admin/config/system/http-response-headers';
}

/**
 * Menu callback for admin/config/system/http-response-headers/%.
 */
function http_response_headers_configure($form, &$form_state, $rule = NULL) {

  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => isset($rule->name) ? $rule->name : '',
    '#description' => t('The human-readable name of this rule. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#disabled' => isset($rule->rule_id) ? TRUE : FALSE,
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['rule_id'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($rule->rule_id) ? $rule->rule_id : '',
    '#maxlength' => 32,
    '#disabled' => isset($rule->rule_id) ? TRUE : FALSE,
    '#machine_name' => array(
      'exists' => 'http_response_headers_load',
    ),
    '#description' => t('A unique machine-readable name for this rule. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['settings']['header'] = array(
    '#type' => 'select',
    '#title' => t('HTTP header'),
    '#description' => '',
    '#default_value' => isset($rule->header) ? $rule->header : '',
    '#options' => variable_get('http_response_headers_allowed_headers', array()),
    '#empty_option' => t('Select a header'),
    '#required' => TRUE,
  );
  $form['settings']['header_value'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP header value'),
    '#maxlength' => 64,
    '#description' => '',
    '#default_value' => isset($rule->header_value) ? $rule->header_value : '',
    '#required' => TRUE,
  );

  // Visibility settings.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'http_response_headers') . '/http_response_headers.js'),
    ),
  );

  // Per-path visibility.
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 0,
  );

  $options = array(
    HTTP_RESPONSE_HEADERS_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    HTTP_RESPONSE_HEADERS_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  ));

  $form['visibility']['path']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Set header on specific pages'),
    '#options' => $options,
    '#default_value' => isset($rule->visibility) ? $rule->visibility : HTTP_RESPONSE_HEADERS_VISIBILITY_NOTLISTED,
  );
  $form['visibility']['path']['pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . t('Pages') . '</span>',
    '#default_value' => isset($rule->pages) ? $rule->pages : '',
    '#description' => $description,
  );

  // Per-node-type visibility.
  $default_type_options = array();
  if (!empty($rule->types)) {
    $default_type_options = explode(',', $rule->types);
  }
  $form['visibility']['node_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 5,
  );
  $form['visibility']['node_type']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Set header for specific content types'),
    '#default_value' => $default_type_options,
    '#options' => node_type_get_names(),
    '#description' => t('Set this header only on pages that display content of the given type(s). If you select no types, there will be no type-specific limitation.'),
  );

  // Per-role visibility.
  $default_role_options = array();
  if (!empty($rule->roles)) {
    $default_role_options = explode(',', $rule->roles);
  }
  $role_options = array_map('check_plain', user_roles());
  $form['visibility']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 10,
  );
  $form['visibility']['role']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Set header for specific roles'),
    '#default_value' => $default_role_options,
    '#options' => $role_options,
    '#description' => t('Set this header only for the selected role(s). If you select no roles, the rule will be visible to all users.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save rule'),
  );

  return $form;
}

/**
 * Form submission handler for http_response_headers_admin_configure().
 */
function http_response_headers_configure_submit($form, &$form_state) {
  if (!form_get_errors()) {
    $form_values = $form_state['values'];
    $fields = array(
      'header' => trim($form_state['values']['header']),
      'header_value' => trim($form_state['values']['header_value']),
      'visibility' => (int) $form_state['values']['visibility'],
      'pages' => trim($form_state['values']['pages']),
      'types' => implode(',', array_filter($form_state['values']['types'])),
      'roles' => implode(',', array_filter($form_state['values']['roles'])),
    );
    http_response_headers_update($form_values['rule_id'], $fields);
    drupal_set_message(t('The header rule configuration has been saved.'));

    // Clear the header rules cache.
    http_response_headers_cache_reset();

    $form_state['redirect'] = 'admin/config/system/http-response-headers';
  }
}

/**
 * Menu callback; delete a single header rule.
 *
 * @ingroup forms
 */
function http_response_headers_delete_confirm($form, &$form_state, $rule) {
  $form['rule_id'] = array('#type' => 'value', '#value' => $rule->rule_id);
  $form['name'] = array('#type' => 'value', '#value' => $rule->name);

  $message = t('Are you sure you want to delete the header rule %type?', array('%type' => $rule->name));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/config/system/http-response-headers', $caption, t('Delete'));
}

/**
 * Process header rule delete confirm submissions.
 *
 * @see http_response_headers_delete_confirm()
 */
function http_response_headers_delete_confirm_submit($form, &$form_state) {
  http_response_headers_delete($form_state['values']['rule_id']);

  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The header rule %name has been deleted.', $t_args));
  watchdog('http_response_headers', 'Deleted header rule %name.', $t_args, WATCHDOG_NOTICE);

  http_response_headers_rebuild();

  $form_state['redirect'] = 'admin/config/system/http-response-headers';
}

/**
 * Callback for configuration form.
 */
function http_response_headers_settings_form($form, &$from_state) {
  $form['http_response_headers']['http_response_headers_allowed_headers'] = array(
    '#type' => 'select',
    '#title' => t('Allowed headers'),
    '#default_value' => variable_get('http_response_headers_allowed_headers', array()),
    '#options' => http_response_headers_get_all_headers(),
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('Allowed headers to set via admin UI on <a href="!header_rule_url"> HTTP response headers page</a> page.', array('!header_rule_url' => 'admin/config/system/http-response-headers')),
  );
  $form['http_response_headers']['http_response_headers_exclude_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude header rule pages'),
    '#rows' => '5',
    '#description' => t('Excludes header rule check on this pages.'),
  );

  $form['http_response_headers']['label'] = array(
    '#type' => 'markup',
    '#markup' => theme('item_list', array(
      'items' => variable_get('http_response_headers_allowed_headers', array()),
      'title' => t('Currently enabled headers:'),
    )),
  );

  return system_settings_form($form);
}

/**
 * Provides response headers.
 *
 * @return array
 *   An array of response headers.
 */
function http_response_headers_get_all_headers() {

  // Standard response headers.
  $headers['Standard response headers'] = array(
    'Access-Control-Allow-Origin',
    'Accept-Ranges',
    'Age',
    'Allow',
    'Cache-Control',
    'Connection',
    'Content-Encoding',
    'Content-Language',
    'Content-Length',
    'Content-Location',
    'Content-MD5',
    'Content-Disposition',
    'Content-Range',
    'Content-Type',
    'Date',
    'ETag',
    'Expires',
    'Last-Modified',
    'Link',
    'Location',
    'P3P',
    'Pragma',
    'Proxy-Authenticate',
    'Refresh',
    'Retry-After',
    'Server',
    'Set-Cookie',
    'Status',
    'Strict-Transport-Security',
    'Trailer',
    'Transfer-Encoding',
    'Vary',
    'Via',
    'Warning',
    'WWW-Authenticate',
  );

  // Non-standard response headers.
  $headers['Common non-standard response headers'] = array(
    'X-Frame-Options',
    'X-XSS-Protection',
    'Content-Security-Policy',
    'X-Content-Security-Policy',
    'X-WebKit-CSP',
    'X-Content-Type-Options',
    'X-Powered-By',
    'X-UA-Compatible',
  );

  // Map key.
  foreach ($headers as $title => $header_set) {
    $return_list[$title] = array_combine($header_set, $header_set);
  }

  return $return_list;
}
