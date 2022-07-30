/* mj-toaster: Begin */
angular.module('mj-toaster', []);

/* default-toaster-template: Begin */
angular.module('mj-toaster').value('mjNotificationTemplates', 'default-toaster-template.html');

angular.module("mj-toaster").run(["$templateCache", function(t) {
    t.put('default-toaster-template.html', '<div class="mj-toaster"><h3 ng-show="title" ng-bind-html="title"></h3><div class="message" ng-bind-html="message"></div></div>');
}])
/* using $templateCache for now default-toaster-template.html would be used later*/
/* default-toaster-template: End */

angular.module('mj-toaster').provider('Notification', function() {

    this.options = {
        delay: 10000,
        startTop: 5,
        startRight: 5,
        verticalSpacing: 5,
        horizontalSpacing: 5,
        positionX: 'right',
        positionY: 'top',
        replaceMessage: false
    };

    this.setOptions = function(options) {
        if (!angular.isObject(options)) throw new Error("Options should be an object!");
        this.options = angular.extend({}, this.options, options);
    };

    this.$get = function($timeout, mjNotificationTemplates, $http, $compile, $templateCache, $rootScope, $injector, $sce, $q) {
        var options = this.options;

        var startTop = options.startTop;
        var startRight = options.startRight;
        var verticalSpacing = options.verticalSpacing;
        var horizontalSpacing = options.horizontalSpacing;
        var delay = options.delay;

        var messageElements = [];

        var notify = function(args, t) {
            var deferred = $q.defer();

            if (typeof args !== 'object') {
                args = {
                    message: args
                };
            }

            args.scope = args.scope ? args.scope : $rootScope;
            args.template = args.template ? args.template : mjNotificationTemplates;
            args.delay = !angular.isUndefined(args.delay) ? args.delay : delay;
            args.type = t ? t : '';
            args.positionY = args.positionY ? args.positionY : options.positionY;
            args.positionX = args.positionX ? args.positionX : options.positionX;
            args.replaceMessage = args.replaceMessage ? args.replaceMessage : options.replaceMessage;

            $http.get(args.template, {
                cache: $templateCache
            }).success(function(template) {

                var scope = args.scope.$new();
                scope.message = $sce.trustAsHtml(args.message);
                scope.title = $sce.trustAsHtml(args.title);
                scope.t = args.type.substr(0, 1);
                scope.delay = args.delay;

                var reposite = function() {
                    var j = 0;
                    var k = 0;
                    var lastTop = startTop;
                    var lastRight = startRight;
                    var lastPosition = [];
                    for (var i = messageElements.length - 1; i >= 0; i--) {
                        var element = messageElements[i];
                        if (args.replaceMessage && i < messageElements.length - 1) {
                            element.addClass('killed');
                            continue;
                        }
                        var elHeight = parseInt(element[0].offsetHeight);
                        var elWidth = parseInt(element[0].offsetWidth);
                        var position = lastPosition[element._positionY + element._positionX];

                        if ((top + elHeight) > window.innerHeight) {
                            position = startTop;
                            k++;
                            j = 0;
                        }

                        var top = (lastTop = position ? position : startTop) + (j === 0 ? 0 : verticalSpacing);
                        var right = lastRight + (k * (horizontalSpacing + elWidth));

                        element.css(element._positionY, top + 'px');
                        element.css(element._positionX, right + 'px');

                        lastPosition[element._positionY + element._positionX] = top + elHeight;

                        j++;
                    }
                };

                var templateElement = $compile(template)(scope);
                templateElement._positionY = args.positionY;
                templateElement._positionX = args.positionX;
                templateElement.addClass(args.type);
                templateElement.bind('webkitTransitionEnd oTransitionEnd otransitionend transitionend msTransitionEnd click', function(e) {
                    e = e.originalEvent || e;
                    if (e.type === 'click' || (e.propertyName === 'opacity' && e.elapsedTime >= 1)) {
                        templateElement.remove();
                        messageElements.splice(messageElements.indexOf(templateElement), 1);
                        reposite();
                    }
                });
                if (angular.isNumber(args.delay)) {
                    $timeout(function() {
                        templateElement.addClass('killed');
                    }, args.delay);
                }

                angular.element(document.getElementsByTagName('body')).append(templateElement);
                var offset = -(parseInt(templateElement[0].offsetHeight) + 50);
                templateElement.css(templateElement._positionY, offset + "px");
                messageElements.push(templateElement);

                scope._templateElement = templateElement;

                scope.kill = function(isHard) {
                    if (isHard) {
                        messageElements.splice(messageElements.indexOf(scope._templateElement), 1);
                        scope._templateElement.remove();
                        $timeout(reposite);
                    } else {
                        scope._templateElement.addClass('killed');
                    }
                };

                $timeout(reposite);

                deferred.resolve(scope);

            }).error(function(data) {
                throw new Error('Template (' + args.template + ') could not be loaded. ' + data);
            });

            return deferred.promise;
        };

        notify.primary = function(args) {
            return this(args, 'primary');
        };
        notify.error = function(args) {
            return this(args, 'error');
        };
        notify.success = function(args) {
            return this(args, 'success');
        };
        notify.info = function(args) {
            return this(args, 'info');
        };
        notify.warning = function(args) {
            return this(args, 'warning');
        };

        notify.clearAll = function() {
            var notifys = angular.element(document.getElementsByClassName('mj-toaster'));

            if (notifys) {
                angular.forEach(notifys, function(notify) {
                    notify.remove();
                });
            }
        };

        return notify;
    };
});
/* mj-toaster: End */



let nic1  = undefined
let nic2 = undefined

let first = 1
$(document).ready(function() {
   angular.element(document.getElementById('choose_nics')).scope().select();
});
var mod = angular.module("FW", ["ngMaterial", "ngMessages", "material.svgAssetsCache", 'mj-toaster']);
mod.controller("tableCtrl", function($scope, Notification) {
    $scope.change = function(col_text, error){
        if (error) {
            Notification.error({
                message:  col_text,
                delay: 5000
            });
        }
        else {
            Notification.success({
                message: 'Valid ' + col_text + '!',
                delay: 5000
            });
        }
    }
});
mod.controller('dataLoader', function($scope, $mdDialog, Notification) {
    $scope.saveJSON = function () {
        //read path from save-textfield
        var src = "rules/"
        var _path = $("#save-textfield").val();
        if (_path.length <= 0) {
            Notification.error({
                message: 'Cannot save when path is empty',
                delay: 5000
            });
            return
        }
        var _fullPath = src + _path;
        var _json = JSON.stringify(globalRuleSet);
        let ret = true;
        if (isGUI){
            ret = SaveRules(_fullPath, _json);
        }
        else{
            var _blob = new Blob([_json], { type: "text/plain;charset=utf-8" });
            var a = document.createElement("a");
            a.href = URL.createObjectURL(_blob);
            a.download = _fullPath;
            //a.click();
        }
        if (ret) {
            Notification.success({
                message: 'Success! saved to ' + _fullPath,
                delay: 5000
            });
        } else {
            Notification.success({
                message: 'Failed to save ' + _fullPath,
                delay: 5000
            });
        }
    };

    $scope.loadJSON = function (ev) {
        let content = "No path specified (confirming will do nothing)";
        if($("#load-textfield").val().length > 0){
            content = "Load rules/" + $("#load-textfield").val() + ".json?";
        }
        var confirm = $mdDialog.confirm()
            .title('Are you sure you want to load rules from a JSON file? Current table will be deleted.')
            .textContent(content)
            .targetEvent(ev)
            .ok('Load')
            .cancel('Cancel');

        $mdDialog.show(confirm).then(function() {
            let ret = true;
            if(isGUI){
                var src = "rules/"
                var _path = $("#load-textfield").val();
                console.log("VALUE: " + _path);
                if (_path.length <= 0) {
                    return
                }
                var _fullPath = src + _path + ".json";
                ret = LoadRules(_fullPath);
                console.log("RET: " + ret);
                if(ret === ""){
                    Notification.error({
                        message: 'Failed to load ' + _fullPath + ": file does not exist",
                        delay: 5000
                    });
                }
                loadJSON(ret);
            }
        }, function() {
        });
    };
});
loader.click();
mod.controller("NICSModal", function ($scope, $mdDialog) {
    //use later for nics vari   ables
    // $scope.status = "  ";
    //the template URL has ng-repeat for each nic
    $scope.nics = [];
    if (isGUI) {
        setTimeout(function (){
            let temp = RequestNICS();
            for (let i = 0; i < temp.length; i++) {
                $scope.nics.push({name: temp[i]});
            }
            first = 0;
        }, (first * 1000))
    }
    else{
        $scope.nics = [
            {
                name: "eth0"
            },
            {
                name: "eth1"
            }
        ];
    }
    $scope.select1 = undefined;
    $scope.select2 = undefined;

    $scope.changeClose = function(ev){
        console.log("1:" + ( $scope.select1 === undefined) + "2:" + ($scope.select2 === undefined) + "3:" + ($scope.select1 === $scope.select2));
        $("#close").attr("disabled", (
           $scope.select1 === undefined ||  $scope.select2 === undefined || $scope.select1 === $scope.select2
        ));
        nic1 = $scope.select1;
        nic2 = $scope.select2;
    };
    $scope.select = function(ev) {
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
            if(isGUI){
                RegisterNICS(nic1.name, nic2.name);
            }
        });
    }
});

mod.controller("ruleModifications", function ($scope, $mdDialog) {

    $scope.add_row = function(ev) {

    }
    $scope.prompt_remove = function(ev) {
        // Appending dialog to document.body to cover sidenav in docs app
        let tempIndices;
        $(".mdl-data-dynamictable tbody").find('tr.is-selected').each(function () {
            var _index = $(this).nextAll().length;
           if (tempIndices === undefined) {
               tempIndices = "Rules: " + 1;
           }
           else{
                tempIndices += ", " +  _index;
           }
        });
        if (tempIndices === undefined) {
            tempIndices = "No rules selected";
        }
        var confirm = $mdDialog.confirm()
            .title('Are you sure you want to delete selected rules?')
            .textContent(tempIndices)
            .targetEvent(ev)
            .ok('Remove Rules')
            .cancel('Cancel');

        $mdDialog.show(confirm).then(function() {
            $(".mdl-data-dynamictable tbody").find('tr.is-selected').each(function () {
                var _index = $(this).nextAll().length;
                $(this).remove();
                if (isGUI){
                    RemoveRule(_index);
                }
                globalRuleSet.splice(_index, 1);
                console.log("Removed rule " + _index);
            });
            $(".mdl-data-dynamictable tbody").find('tr').each(function () {
                var _index = $(this).nextAll().length;
                $(this).find("span.index").text(_index);
            });
            $(".mdl-data-dynamictable thead tr").removeClass("is-selected");
            $(".mdl-data-dynamictable thead tr th label").removeClass("is-checked");
            componentHandler.upgradeDom();
            var _row = $(".mdl-data-dynamictable tbody").find('tr');
            if (_row.length < 1) {
                addNewRow();
            }
        }, function() {
        });
    };

});