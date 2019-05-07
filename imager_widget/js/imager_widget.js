(function ($, Drupal) {
  Drupal.behaviors.imagerWidget = {
    attach: function (context, settings) {

      var iw_init = true;

      var pluginsConfig = {
        Rotate: {
          controlsCss: {
            width: '15px',
            height: '15px',
            background: 'white',
            border: '1px solid black'
          },
          controlsTouchCss: {
            width: '25px',
            height: '25px',
            background: 'white',
            border: '2px solid black'
          }
        },
        Resize: {
          controlsCss: {
            width: '15px',
            height: '15px',
            background: 'white',
            border: '1px solid black'
          },
          controlsTouchCss: {
            width: '25px',
            height: '25px',
            background: 'white',
            border: '2px solid black'
          }
        },
        Crop: {
          controlsCss: {
            width: '15px',
            height: '15px',
            background: 'white',
            border: '1px solid black'
          },
          controlsTouchCss: {
            width: '25px',
            height: '25px',
            background: 'white',
            border: '2px solid black'
          }
        },
        Toolbar: {
          toolbarSize: 85,
          toolbarSizeTouch: 43,
          tooltipEnabled: true,
          tooltipCss: {
            color: 'white',
            background: 'black'
          }
        },
        Save: {
          upload: true,
          uploadFunction: function (imageId, imageData, callback) {
            $('.image-widget-data textarea').val(imageData);
            alert('Your edits have been saved. Be sure to click the Save button at the bottom of the page to finish saving this photo.')
          }
        }
      };

      var options = {
        plugins: ['Rotate', 'Crop', 'Save', 'Toolbar', 'Undo'],
        editModeCss: {
        },
        pluginsConfig: pluginsConfig,
        contentConfig: {
          saveImageData: function (imageId, imageData) {
            try {
              console.log('Save button clicked! ImageId:', imageId);
              console.log('ImageData argument here is the image encoded in base64 string. ' +
                'This function gets called anytime user clicks on `save` button. ' +
                'If one wants to disable edit after saving, check the `standalone-remote-upload.html` ' +
                'example file which shows how to upload image on the server ' +
                'and display it in place of ImagerJs after that.');
              localStorage.setItem('image_' + imageId, imageData);
            } catch (err) {
              console.error(err);
            }
          },           
          loadImageData: function (imageId) {
            return true;
          }
        }
      };

      var addNew = function () {
        var image_preview_src = $('.form-type-managed-file .image-preview img').attr('src');
        var image_src_pieces = image_preview_src.split("/public/");
        var image_src = '/sites/default/files/' + image_src_pieces[1];
        toDataUrl(image_src, function(base64_code) {
          var imagerid = generateUUID();
          var $imageContainer = $(
            '<div class="image-container">' +
            '  <img data-imager-id="' + imagerid + '" class="imager-original" ' +
            '       src="' + base64_code + '" ' +
            '       style="min-width: 300px; min-height: 200px; width: 300px;">' +
            '</div>');

          if($('#imagers .image-container').length == 0) {
            $('#imagers').append($imageContainer);
            var imager = new ImagerJs.Imager($imageContainer.find('img.imager-original'), options);
            imager.$imageElement.on('load', function (event) {
              if (iw_init) {
                iw_init = false;
                imager.handleImageElementSrcChanged();
                imager.startEditing();
              }
            });

            imager.on('editStart', function () {
              // fix image dimensions so that it could be properly placed on the grid
              imager.$imageElement.css({
                minWidth: 'auto',
                minHeight: 'auto'
              });
            });

          }
        });
      };

      $('.add-new-imager').on('click', function(e) {
        e.preventDefault();
        addNew();
      });

      function toDataUrl(url, callback) {
        var xhr = new XMLHttpRequest();
        xhr.onload = function() {
            var reader = new FileReader();
            reader.onloadend = function() {
                callback(reader.result);
            }
            reader.readAsDataURL(xhr.response);
        };
        xhr.open('GET', url);
        xhr.responseType = 'blob';
        xhr.send();
      }

      function generateUUID() {
        var d = new Date().getTime();
        var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
          var r = (d + Math.random() * 16) % 16 | 0;
          d = Math.floor(d / 16);
          return (c == 'x' ? r : (r & 0x3 | 0x8)).toString(16);
        });
        return uuid;
      }

    }
  };
})(jQuery, Drupal);
