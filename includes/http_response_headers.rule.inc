<?php
/**
 * @file
 * Class definitions for the HTTP response headers rule.
 */

/**
 * Defines HttpResponseHeadersRule
 */
class HttpResponseHeadersRule {
  public $machine_name;
  public $description;
  public $header;
  public $header_value;
  public $pages;
  public $visibility;
  public $types;
  public $roles;
  public $export_type;

  /**
   * Constructs new HttpResponseHeadersRule object.
   */
  public function __construct() {
    // Initialise defaults.
    $this->visibility = HTTP_RESPONSE_HEADERS_VISIBILITY_NOTLISTED;
  }

  /**
   * Updates roles.
   *
   * @param array $roles
   *   An array of role with ID.
   */
  public function updateRoles($roles = array()) {
    if (!empty($roles)) {
      $this->roles = implode(',', array_filter($roles));
    }
  }

  /**
   * Updates the content types.
   *
   * @param array $types
   *   An array of content type with key.
   */
  public function updateTypes($types = array()) {
    if (!empty($types)) {
      $this->types = implode(',', array_filter($types));
    }
  }

}
