<?php
/**
 * @file
 * Class definitions for the HTTP response headers rule.
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

  public function __construct() {
    // Initialise defaults.
    $this->description = '';
    $this->header = '';
    $this->headerV_value = '';
    $this->pages = '';
    $this->visibility = HTTP_RESPONSE_HEADERS_VISIBILITY_NOTLISTED;
    $this->roles = '';
    $this->types = '';
  }

  public function updateRoles($roles = array()) {
    if (!empty($roles)) {
      $this->roles = implode(',', array_filter($roles));
    }
  }

  public function updateTypes($types = array()) {
    if (!empty($types)) {
      $this->types = implode(',', array_filter($types));
    }
  }

  /**
   * Returns any extra data to be exported when exporting to a string.
   *
   * If your subclass has more items you want to export than base class, please
   * export them here.
   *
   * @return mixed $output.
   */
  public function exportData() {
    $output = '';
    return $output;
  }

}
