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

    $scope.name = 'Test';

    $scope.selectedCommand = 'AdminKill';

    $scope.init = function(){
      $scope.device = DeviceFactory.getDevice($stateParams.id);
    }

    $scope.commands = ['AdminKill','FullAmmo', 'SetName','GetPlayerName'];

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
      alert('Set name of tagger to: ' + $scope.name);
      ble.write(
        $stateParams.id,
        service_id,
        characteristic_id,
        btoa("AT+VERSION"),
        bleSuccess,
        bleError
      );
    }

    $scope.getPlayerName = function(){
      ble.read(
        $stateParams.id,
        service_id,
        characteristic_id,
        btoa("AT+VERSION"),
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
        console.log(response);
        alert(response);
        //ble.disconnect($stateParams.id);
      }else{
        alert.log("We did not get an OK from the ble device");

      }
    }

    function bleError(err){
      alert.log(err);
      alert("Error occured while trying to send te command");
    }

  }

})();
