/**
 * Tabs plugin for jQuery created by Òscar Casajuana < elboletaire at underave dot net >
 *
 * @copyright Copyright 2013-2016 Òscar Casajuana
 * @license   MIT
 * @author    Òscar Casajuana Alonso <elboletaire at underave dot net>
*/
;(function($, document, window, undefined) {
  "use strict";

  var Tabbedcontent = function(tabcontent, options) {
    var defaults = {
        links         : tabcontent.prev().find('a').length ? tabcontent.prev().find('a') : '.tabs a', // the tabs itself. By default it selects the links contained in the previous wrapper or the links inside ".tabs a" if there's no previous item
        errorSelector : '.error-message', // false to disable
        speed         : false, // speed of the show effect. Set to null or false to disable
        onSwitch      : false, // onSwitch callback
        onInit        : false, // onInit callback
        currentClass  : 'active', // current selected tab class (is set to the <a> element)
        tabErrorClass : 'has-errors', // a class to be added to the tab where errorSelector is detected
        history       : true, // set to false to disable HTML5 history
        historyOnInit : true, // allows to deactivate the history for the intial autmatically tab switch on load
        loop          : false // if set to true will loop between tabs when using the next() and prev() api methods
      },
      firstTime = false,
      children  = tabcontent.children(),
      history   = window.history,
      loc       = document.location,
      current   = null
    ;

    options = $.extend(defaults, options);

    if (!(options.links instanceof $)) {
      options.links = $(options.links);
    }

    /**
     * Checks if the specified tab id exists.
     *
     * @param  string tab Tab #id
     * @return bool
     */
    function tabExists(tab) {
      return Boolean(children.filter(tab).length);
    }
    /**
     * Checks if the current tab is the
     * first one in the tabs set.
     *
     * @return bool
     */
    function isFirst() {
      return current === 0;
    }
    /**
     * Checks if the passed number is an integer.
     *
     * @param  mixed  num The value to be checked.
     * @return bool
     */
    function isInt(num) {
      return num % 1 === 0;
    }
    /**
     * Checks if the current tab is the
     * last one in the tabs set.
     *
     * @return {Boolean} [description]
     */
    function isLast() {
      return current === children.length - 1;
    }
    /**
     * Filters a tab based on current links href.
     *
     * Method for compatibility with Zepto.js
     *
     * @param  string tab Tab #href
     * @return bool
     */
    function filterTab(tab) {
      return $(this).attr('href').match(new RegExp(tab + '$'));
    }
    /**
     * Returns an object containing two jQuery instances:
     * one for the tab content and the other for its link.
     *
     * @param  mixed tab A tab id, #id or index.
     * @return object    With thi
     */
    function getTab(tab) {
      if (tab instanceof $) {
        return {
          tab : tab,
          link : options.links.eq(tab.index())
        };
      }
      if (isInt(tab)) {
        return {
          tab : children.eq(tab),
          link : options.links.eq(tab)
        };
      }
      if (children.filter(tab).length) {
        return {
          tab : children.filter(tab),
          link : options.links.filter(function() {
            return filterTab.apply(this, [tab]);
          })
        };
      }
      // assume it's an id without #
      return {
        tab : children.filter('#' + tab),
        link : options.links.filter(function() {
          return filterTab.apply(this, ['#' + tab]);
        })
      };
    }
    /**
     * Returns the index of the current tab.
     *
     * @return int
     */
    function getCurrent() {
      return options.links.parent().filter('.' + options.currentClass).index();
    }
    /**
     * Go to the next tab in the tabs set.
     *
     * @param  bool  loop If defined will overwrite options.loop
     * @return mixed
     */
    function next(loop) {
      ++current;

      if (loop === undefined) loop = options.loop;

      if (current < children.length) {
        return switchTab(current, true);
      } else if (loop && current >= children.length) {
        return switchTab(0, true);
      }

      return false;
    }
    /**
     * Go to the previous tab in the tabs set.
     *
     * @param  bool  loop If defined will overwrite options.loop
     * @return mixed
     */
    function prev(loop) {
      --current;

      if (loop === undefined) loop = options.loop;

      if (current >= 0) {
        return switchTab(current, true);
      } else if (loop && current < 0) {
        return switchTab(children.length - 1, true);
      }

      return false;
    }
    /**
     * onSwitch callback for switchTab.
     *
     * @param  string tab The tab #id
     * @return void
     */
    function onSwitch(tab) {
      if (options.history && options.historyOnInit && firstTime && history !== undefined && ('pushState' in history)) {
        firstTime = false;
        window.setTimeout(function() {
          history.replaceState(null, '', tab);
        }, 100);
      }
      current = getCurrent();
      if (options.onSwitch && typeof options.onSwitch === 'function') {
        options.onSwitch(tab, api());
      }
      tabcontent.trigger('tabcontent.switch', [tab, api()]);
    }
    /**
     * Switch to specified tab.
     *
     * @param  mixed tab The tab to switch to.
     * @param  bool  api Set to true to force history writing.
     * @return bool      Returns false if tab does not exist; true otherwise.
     */
    function switchTab(tab, api) {
      if (!tab.toString().match(/^#/)) {
        tab = '#' + getTab(tab).tab.attr('id');
      }

      if (!tabExists(tab)) {
        return false;
      }

      // Toggle active class
      options.links.attr('aria-selected','false').parent().removeClass(options.currentClass);
      options.links.filter(function() {
        return filterTab.apply(this, [tab]);
      }).attr('aria-selected','true').parent().addClass(options.currentClass);
      // Hide tabs
      children.hide();

      // We need to force the change of the hash if we're using the API
      if (options.history && api) {
        if (history !== undefined && ('pushState' in history)) {
          history.pushState(null, '', tab);
        } else {
          // force hash change to add it to the history
          window.location.hash = tab;
        }
      }

      // Show tabs
      children.attr('aria-hidden','true').filter(tab).show(options.speed, function() {
        if (options.speed) {
          onSwitch(tab);
        }
      }).attr('aria-hidden','false');
      if (!options.speed) {
        onSwitch(tab);
      }

      return true;
    }
    /**
     * Api method to switch tabs.
     *
     * @param  mixed tab Tab to switch to.
     * @return bool      Returns false if tab does not exist; true otherwise.
     */
    function apiSwitch(tab) {
      return switchTab(tab, true);
    }
    /**
     * Method used to switch tabs using the
     * browser query hash.
     *
     * @param  object e Event.
     * @return void
     */
    function hashSwitch(e) {
      switchTab(loc.hash);
    }
    /**
     * Initialization method.
     *
     * The tab checking preference is:
     *   - document.location.hash
     *   - options.errorSelector
     *   - first tab in the set of tabs
     *
     * The onInit method is called at the
     * end of this method.
     *
     * @return void
     */
    function init() {
      // Switch to tab using location.hash
      if (tabExists(loc.hash)) {
        // Switch to current hash tab
        switchTab(loc.hash);
      }
      // If there's a tab link with the options.currentClass set,
      // switch to that tab.
      else if (options.links.parent().filter('.' + options.currentClass).length) {
        switchTab(options.links.parent().filter('.' + options.currentClass).index());
      }
      // Switch to tab containing class options.errorSelector
      else if (options.errorSelector && children.find(options.errorSelector).length) {
        // Search for errors and show first tab containing one
        children.each(function() {
          if ($(this).find(options.errorSelector).length) {
            switchTab("#" + $(this).attr("id"));
            return false;
          }
        });
      }
      // Open first tab
      else {
        switchTab("#" + children.filter(":first-child").attr("id"));
      }
      // Add a class to every tab containing errors
      if (options.errorSelector) {
        children.find(options.errorSelector).each(function() {
          var tab = getTab($(this).parent());
          tab.link.parent().addClass(options.tabErrorClass);
        });
      }

      // Binding
      if ('onhashchange' in window) {
        $(window).bind('hashchange', hashSwitch);
      } else { // old browsers
        var current_href = loc.href;
        window.setInterval(function() {
          if (current_href !== loc.href) {
            hashSwitch.call(window.event);
            current_href = loc.href;
          }
        }, 100);
      }
      // Bind click event on links, to ensure we don't rewrite the URI in
      // case history is disabled
      $(options.links).on('click', function(e) {
        switchTab($(this).attr('href').replace(/^[^#]+/, ''), options.history);
        e.preventDefault();
      });

      // onInit callback
      if (options.onInit && typeof options.onInit === 'function') {
        options.onInit(api());
      }
      tabcontent.trigger('tabcontent.init', [api()]);
    }
    /**
     * Returns the methods exposed in the api.
     *
     * @return object Containing each api method.
     */
    function api() {
      return {
        'switch'       : apiSwitch,
        'switchTab'    : apiSwitch, // for old browsers
        'getCurrent'   : getCurrent,
        'getTab'       : getTab,
        'next'         : next,
        'prev'         : prev,
        'isFirst'      : isFirst,
        'isLast'       : isLast
      };
    }

    init();

    return api();
  };

  $.fn.tabbedContent = function(options) {
    return this.each(function() {
      var tabs = new Tabbedcontent($(this), options);
      $(this).data('api', tabs);
    });
  };

})(window.jQuery || window.Zepto || window.$, document, window);
