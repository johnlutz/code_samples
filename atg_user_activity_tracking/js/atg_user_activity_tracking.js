(function($) {
  Drupal.behaviors.atgUserActivityTracking = {
    attach: function(context, settings) {
      // Initialize Riveted for user idleness tracking.
      riveted.init({
        reportInterval: 30,
        idleTimeout: 1800,
        eventHandler: function(data) {
          var idleTime = Drupal.settings.atg_user_activity_tracking.idle_time;
          if (data >= idleTime) {
            $.get("/user-tracking/insert-idle");
          }
        }
      });
    }
  };
})(jQuery);
