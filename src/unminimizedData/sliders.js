/* -----------------------------------------------------
  Material Design Sliders
  CodePen URL: https://codepen.io/rkchauhan/pen/xVGGpR
  By: Ravikumar Chauhan

  Find me on:-
  * Twitter: https://twitter.com/rkchauhan01
  * Facebook: https://www.facebook.com/ravi032chauhan
  * GitHub: https://github.com/rkchauhan
  * CodePen: https://codepen.io/rkchauhan
  * UpLabs: http://uplabs.com/rkchauhan01

  Thanks to:-
  * Google Material design - https://www.google.com/design/spec/material-design/introduction.html
  * Google Material Color - https://www.google.com/design/spec/style/color.html
  * Google Material Icons - https://design.google.com/icons/
  * Roboto Font - https://google.com/fonts/specimen/Roboto
  * jQuery - https://jquery.com
-------------------------------------------------------- */
$(document).ready(function() {

  $('.rkmd-slider').rkmd_rangeSlider();
  change_slider_color();

});
$(document).ready(function () {
  //your code here
/* Range Slider Function */
(function($) {

  $.fn.rkmd_rangeSlider = function() {
    var self, slider_width, slider_offset, curnt, sliderContinuous, sliderDiscrete, range, slider;
    self             = $(this);
    slider_width     = self.outerWidth();
    slider_offset    = self.offset().left;
    sliderDiscrete   = $('.slider-discrete');

    if(self.hasClass('slider-discrete') === true) {

      sliderDiscrete.each(function(i, v) {
        curnt         = $(this);
        curnt.append(sliderDiscrete_tmplt());
        range         = curnt.find('input[type="range"]');
        slider        = curnt.find('.slider');
        slider_fill   = slider.find('.slider-fill');
        slider_handle = slider.find('.slider-handle');
        slider_label  = slider.find('.slider-label');

        var range_val = parseInt(range.val());
        slider_fill.css('width', range_val +'%');
        slider_handle.css('left', range_val +'%');
        slider_label.find('span').text(range_val);
        console.log(range_val);
      });
    }

    self.on('mousedown', '.slider-handle', function(e) {
      if(e.button === 2) {
        return false;
      }

      var parents       = $(this).parents('.rkmd-slider');
      var slider_width  = parents.outerWidth();
      var slider_offset = parents.offset().left;
      var check_range   = parents.find('input[type="range"]').is(':disabled');

      if(check_range === true) {
        return false;
      }

      if(parents.hasClass('slider-discrete') === true) {
        $(this).addClass('is-active');
      }
      var handlers = {
        mousemove: function(e) {
          var slider_new_width = e.pageX - slider_offset;

          if(slider_new_width <= slider_width && !(slider_new_width < '0')) {
            slider_move(parents, slider_new_width, slider_width);
          }
        },
        mouseup: function(e) {
          $(this).off(handlers);

          if(parents.hasClass('slider-discrete') === true) {
            parents.find('.is-active').removeClass('is-active');
          }
        }
      };
      $(document).on(handlers);
    });

    self.on('mousedown', '.slider', function(e) {
      if(e.button === 2) {
        return false;
      }

      var parents       = $(this).parents('.rkmd-slider');
      var slider_width  = parents.outerWidth();
      var slider_offset = parents.offset().left;
      var check_range   = parents.find('input[type="range"]').is(':disabled');

      if(check_range === true) {
        return false;
      }

      var slider_new_width = e.pageX - slider_offset;
      if(slider_new_width <= slider_width && !(slider_new_width < '0')) {
        slider_move(parents, slider_new_width, slider_width);
      }

      var handlers = {
        mouseup: function(e) {
          $(this).off(handlers);
        }
      };
      $(document).on(handlers);

    });
  };

  function sliderDiscrete_tmplt() {
    var tmplt = '<div class="slider">' +
        '<div class="slider-fill"></div>' +
        '<div class="slider-handle"><div class="slider-label"><span>0</span></div></div>' +
        '</div>';

    return tmplt;
  }

  function slider_move(parents, newW, sliderW) {
    var slider_new_val = parseInt(Math.round(newW / sliderW * 100));

    var slider_fill    = parents.find('.slider-fill');
    var slider_handle  = parents.find('.slider-handle');
    var range          = parents.find('input[type="range"]');

    slider_fill.css('width', slider_new_val +'%');
    slider_handle.css({
      'left': slider_new_val +'%',
      'transition': 'none',
      '-webkit-transition': 'none',
      '-moz-transition': 'none'
    });

    range.val(slider_new_val);

    if(parents.hasClass('slider-discrete') === true) {
      parents.find('.slider-handle span').text(slider_new_val);
    }
  }

}(jQuery));


/* Change Slider Color
function change_slider_color() {
  $('.color-box .show-box').on('click', function() {
    $(".color-box").toggleClass("open");
  });

  $('.colors-list a').on('click', function() {
    var curr_color = $('main').data('slider-color');
    var color      = $(this).data('slider-color');
    var new_colot  = 'slider-' + color;

    $('.rkmd-slider').each(function(i, v) {
      var findColor = $(this).hasClass(curr_color);

      if(findColor) {
        $(this).removeClass(curr_color);
        $(this).addClass(new_colot);
      }

      $('main').data('slider-color', new_colot);

    });
  });
}
*/
});
