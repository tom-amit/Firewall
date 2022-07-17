angular
    .module("FW", ["ngMaterial", "ngMessages", "material.svgAssetsCache"])

    .controller("NICSModal", function ($scope, $mdDialog) {
        //use later for nics variables
        // $scope.status = "  ";
        $scope.NICS = ["1", '2'];
        $scope.select = function() {
            $mdDialog.show({
               //template to have 2 dropdowns for nics
                templateUrl: 'scripts/templates/nics_modal.html',
                controller: function DialogController($scope, $mdDialog) {
                    $scope.closeDialog = function() {
                        $mdDialog.hide();
                    }
                }
            }).then(function() {
                alert("confirmation");
            }, function() {
                alert("annulation");
            });
    }
});