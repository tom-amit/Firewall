let nic1  = undefined
let nic2 = undefined
$(document).ready(function() {
    angular.element(document.getElementById('choose_nics')).scope().select();
});
angular
    .module("FW", ["ngMaterial", "ngMessages", "material.svgAssetsCache"])

    .controller("NICSModal", function ($scope, $mdDialog) {
        //use later for nics variables
        // $scope.status = "  ";
        //the template URL has ng-repeat for each nic
        $scope.nics = [
            {
                name: "eth0"
            },
            {
                name: "eth1"
            }
        ];
        $scope.select1 = undefined;
        $scope.select2 = undefined;

        $scope.changeClose = function(){
            console.log("1:" + ( $scope.select1 === undefined) + "2:" + ($scope.select2 === undefined) + "3:" + ($scope.select1 === $scope.select2));
            $("#close").attr("disabled", (
               $scope.select1 === undefined ||  $scope.select2 === undefined || $scope.select1 === $scope.select2
            ));
            nic1 = $scope.select1;
            nic2 = $scope.select2;
        };
        $scope.select = function() {
            $mdDialog.show({
                escapeToClose: false,
                clickOutsideToClose: false,
                backdropClickToClose: false,
                templateUrl: 'scripts/templates/nics_modal.html',
                controller: function DialogController($scope, $mdDialog) {
                    $scope.closeDialog = function() {
                        $mdDialog.hide();
                    }
                },
                onComplete: function(){
                    nic1 = undefined;
                    nic2 = undefined;
                    $scope.changeClose();
                }
            }).then(function() {
                $("#nic_1").text("First Network Card: " + nic1.name);
                $("#nic_2").text("Second Network Card: " + nic2.name);
            });
        }
    });