<?php

namespace Drupal\calzoo_contests\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Url;

class ContestController extends ControllerBase {
  public function content() {
    $items[] = [
      'title' => 'Summer 2018 Contest Configuration',
      'url' => Url::fromRoute('calzoo_contests.summer_2018_settings'),
      'description' => 'Configuration of Summer 2018 contest.',
      'localized_options' => [],
    ];
    return [
      '#theme' => 'admin_block_content',
      '#content' => $items,
    ];
  }
}