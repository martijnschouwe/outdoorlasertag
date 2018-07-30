(function(){
  angular.module('starter')
  .controller('DeviceController', ['$scope', '$state', '$stateParams', 'DeviceFactory', DeviceController]);

  function DeviceController($scope, $state, $stateParams, DeviceFactory){

    var me = this;

    //got these from an android app called BLE Scanner in which you can view the ble uuid's etc
    var service_id = '0000FFE0-0000-1000-8000-00805F9B34FB';
    var characteristic_id = '0000FFE1-0000-1000-8000-00805F9B34FB'; //custom characteristics

    me.player = {
      name: ''
    }

    $scope.name = '';

    $scope.selectedCommand = '';

    $scope.init = function(){
      $scope.device = DeviceFactory.getDevice($stateParams.id);
    }

    $scope.commands = ['AdminKill','FullAmmo', 'SetName'];

    $scope.sendCommand = function(){
      console.log('The selected command is : ',$scope.selectedCommand);
      ble.write(
        $stateParams.id,
        service_id,
        characteristic_id,
        btoa(JSON.stringify({command: $scope.selectedCommand})),
        bleSuccess,
        bleError
      );
    };

    $scope.setNameOnBLEDevice = function(){
      console.info('Set name of tagger to: ' + $scope.name);
      ble.write(
        $stateParams.id,
        service_id,
        characteristic_id,
        btoa("AT+NAME"+$scope.name),
        bleSuccess,
        bleError
      );
    }

    $scope.backToHome = function(){
      $state.go('home');
      ble.disconnect($stateParams.id);
    }

    function bleSuccess(response){
      if(response == 'OK'){
        alert("Command send to ble device");
        ble.disconnect($stateParams.id);
      }else{
        console.log("We did not get an OK from the ble device");

      }
    }

    function bleError(err){
      console.log(err);
      alert("Error occured while trying to send te command");
    }

  }

})();
