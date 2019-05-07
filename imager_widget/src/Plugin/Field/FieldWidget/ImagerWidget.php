<?php

namespace Drupal\imager_widget\Plugin\Field\FieldWidget;

use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\ReplaceCommand;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Config\Entity\ConfigEntityStorageInterface;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Render\ElementInfoManagerInterface;
use Drupal\image\Plugin\Field\FieldWidget\ImageWidget;
use Drupal\image_widget_crop\ImageWidgetCropInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Render\Element\Select;

/**
 * Plugin implementation of the 'imager_widget' widget.
 *
 * @FieldWidget(
 *   id = "imager_widget",
 *   label = @Translation("Imager Widget"),
 *   field_types = {
 *     "image"
 *   }
 * )
 */
class ImagerWidget extends ImageWidget {

  /**
   * The config factory.
   *
   * Subclasses should use the self::config() method, which may be overridden to
   * address specific needs when loading config, rather than this property
   * directly. See \Drupal\Core\Form\ConfigFormBase::config() for an example of
   * this.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * Form API callback: Processes a crop_image field element.
   *
   * Expands the image_image type to include the alt and title fields.
   *
   * This method is assigned as a #process callback in formElement() method.
   *
   * @return array
   *   The elements with parents fields.
   */
  public static function process($element, FormStateInterface $form_state, $form) {
    if ($element['#files']) {
      foreach ($element['#files'] as $file) {
        $element['imager_widget_wrapper'] = [
          '#type' => 'fieldset',
          '#title' => t('Edit your image'),
        ];
        $element['imager_widget_wrapper']['imager_widget'] = [
          '#markup' => '<a class="add-new-imager button" href="#">' . t('Start Editing') . '</a><div class="description">' . t('To rotate or crop image: IMPORTANT: Click \'Save\' icon at top right corner of the image to save your edited photo. Click \'Save\' button at the bottom of profile to save the profile before navigating away from the page. ') . '</div>
            <div id="imagers"></div>',
        ];
        $element['imager_widget_wrapper']['imager_output'] = [
          '#type' => 'textarea',
          '#prefix' => '<div class="hidden">',
          '#suffix' => '</div>',
        ];
      }
      $element['#attached']['library'][] = 'imager_widget/imagerjs';
    }

    return parent::process($element, $form_state, $form);
  }

  /**
   * {@inheritdoc}
   */
  public function massageFormValues(array $values, array $form, FormStateInterface $form_state) {

    if (!empty($values[0]['imager_widget_wrapper']['imager_output']) && !empty($values[0]['fids'][0])) {
      // Get original file.
      $original_file = file_load($values[0]['fids'][0]);
      $original_file_name = $original_file->getFilename();

      // Get data from saved image data.
      list($type, $data) = explode(';', $values[0]['imager_widget_wrapper']['imager_output']);
      list(, $data) = explode(',', $data);
      $data_decoded = base64_decode($data);
      list(, $mimetype) = explode(':', $type);

      // Save new file to drupal file managed.
      $file = file_save_data($data_decoded, 'public://' . $original_file_name, FILE_EXISTS_RENAME);
      $file->setMimeType($mimetype);
      $file->save();

      // Set new fid to field values.
      if (is_object($file)) {
        $values[0]['fids'][0] = $file->id();
        unset($values[0]['imager_output']);
      }
    }

    return parent::massageFormValues($values, $form, $form_state);
  }

}
