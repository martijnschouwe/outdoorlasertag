(function(){
  angular.module('starter')
    .controller('StartGameController', ['$scope', '$state', '$stateParams', 'DeviceFactory', StartGameController]);

  function StartGameController($scope, $state, $stateParams, DeviceFactory){

    var me = this;

    $scope.sendCommand = function(){
      console.log('The selected command is : ',$scope.selectedCommand);
      ble.write(
        $stateParams.id,
        service_id,
        characteristic_id,
        btoa(JSON.stringify({command: $scope.selectedCommand})),
        function(response){
          if(response == 'OK'){
            alert("Command send to ble device");
            ble.disconnect($stateParams.id);
          }
        },
        function(err){
          console.log("***", err);
          alert("Error occured while trying to send te command");
        }
      );
    };


    $scope.backToHome = function(){
      $state.go('home');
      ble.disconnect($stateParams.id);
    }

  }

})();
