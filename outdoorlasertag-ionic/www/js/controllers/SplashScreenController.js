(function() {
  'use strict';
  angular
    .module('starter')
    .controller('SplashScreenController',['$cordovaSplashscreen', SplashScreenController] );

  function SplashScreenController($cordovaSplashscreen){
    console.log("Show splash");

      $cordovaSplashscreen.show();
      $cordovaSplashscreen.hide();
  }
})();
