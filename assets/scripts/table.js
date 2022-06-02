//TODO: angular needs to be downloaded locally, not remotely
const isGUI = false;
const titles = ["Name", "Direction", "Source IP", "Destination IP", "Source Port", "Destination Port", "Protocol", "ACK", "Action"];
const n = titles.length;
var loader = document.createElement('input');
loader.type = 'file';
function createTemplate(){
    let dict = {};
    //iterate over titles and insert to arrTemplate with a null value
    for(let i = 0; i < titles.length; i++){
        dict[titles[i]] = null;
    }
    return dict
}
var globalRuleSet = [];
angular.module('modalTest', ['ui.bootstrap', 'dialogs'])
    .controller('dialogServiceTest', function ($scope, $rootScope, $timeout, $dialogs) {
        $scope.launch = function () {
            var dlg = null;
            dlg = $dialogs.confirm('Please Confirm', 'Are you sure you want to deleted selected rules?');
            dlg.result.then(function (btn) {
                $(".mdl-data-dynamictable tbody").find('tr.is-selected').each(function () {
                    var _index = $(this).prevAll().length;
                    $(this).remove();
                    if (isGUI){
                        RemoveRule(_index);
                    }
                    globalRuleSet.splice(_index, 1);
                    console.log("Removed rule " + _index);
                });
                $(".mdl-data-dynamictable tbody").find('tr').each(function () {
                    var _index = $(this).prevAll().length;
                    $(this).find("span.index").text(_index);
                });
                $(".mdl-data-dynamictable thead tr").removeClass("is-selected");
                $(".mdl-data-dynamictable thead tr th label").removeClass("is-checked");
                componentHandler.upgradeDom();
                var _row = $(".mdl-data-dynamictable tbody").find('tr');
                if (_row.length < 1) {
                    addNewRow();
                }
            }, function (btn) {
            });
        }; // end launch

        $scope.saveJSON = function () {

            var _json = JSON.stringify(globalRuleSet);
            var _blob = new Blob([_json], { type: "text/plain;charset=utf-8" });
            saveAs(_blob, "rules.json");
        };

        $scope.loadJSON = function () {
            console.log("LOAD");
            //open a modal, and if user clicks no, return
            var dlg = null;
            dlg = $dialogs.confirm('Please Confirm', 'Are you sure you want to load rules from a JSON file? Current table will be deleted.');
            //return if user clicks no
            dlg.result.then(function (btn) {
                loader.click();
            });
        };
    }); // end run / module

loader.onchange = function (e) {
    var file = e.target.files[0];
    console.log("FILE: " + file);
    //read file as a JSON object
    var reader = new FileReader();
    reader.onload = function (e) {
        let _rules = JSON.parse(e.target.result);
        //delete all rows except the last one
        $(".mdl-data-dynamictable tbody").find("tr:not(:last)").each(function () {
            var _index = $(this).prevAll().length;
            $(this).remove();
        });
        globalRuleSet = [];
        for (let i = 0; i < _rules.length; i++) {
            // unpack _rules[i] into an array and send to addReaRow
            var _arr = [];
            for (let j = 0; j < n; j++) {
                _arr.push(_rules[i][titles[j]]);
            }
            addReadyRow(_arr);
        }
        loader.value="";
    }
    reader.readAsText(file);
};
function saveAs(blob, fileName) {
    var a = document.createElement("a");
    a.href = URL.createObjectURL(blob);
    a.download = fileName;
    a.click();
    URL.revokeObjectURL(a.href);
}

function addReadyRow(ruleArray) {
            var _row = $(".mdl-data-dynamictable tbody").find('tr');
            var template = $('#basketItemTemplateReady').html();
            var _newRow = template.replace(/{{index_rule}}/g, globalRuleSet.length);
            var _newRow = _newRow.replace(/{{id}}/gi, 'checkbox-' + new Date().getTime());
            for (let i = 0; i < n; i++) {
                text = ruleArray[i];
                if (text === null) {
                    text = "Add " + titles[i];
                }
                console.log("ruleArray[" + i + "] = " + text);

                _newRow = _newRow.replace(new RegExp("{{" + titles[i] + "}}", "gi"), text);
            }
            globalRuleSet.push(createTemplate());
            for (let i = 0; i < n; i++) {
                globalRuleSet[globalRuleSet.length - 1][titles[i]] = ruleArray[i];
            }
            var check = false;
            for (var key in globalRuleSet[globalRuleSet.length - 1]) {
                if (globalRuleSet[globalRuleSet.length-1][key] === null) {
                    check = true;
                    break;
                }
            }
            var ret = true;
            if (isGUI){
                ret = addRule(ruleArray[0], ruleArray[1], ruleArray[2], ruleArray[3], ruleArray[4], ruleArray[5], ruleArray[6], ruleArray[7], ruleArray[8]);
            }
            if (ret === false) {
                console.log("Ready rule addition failed!");
            }
            else{
                console.log("Ready rule addition succeeded!");
            }
            if (!check && !ret) {
                //remove "failed" class from _newRow
                _newRow = _newRow.replace(/failed/gi, "");
            }
            $(".mdl-data-dynamictable tbody tr:last").before(_newRow);
            componentHandler.upgradeAllRegistered();
        }

function addNewRow() {

    var _row = $(".mdl-data-dynamictable tbody").find('tr');
    var template = $('#basketItemTemplate').html();
    var _newRow = template.replace(/{{index_rule}}/g, globalRuleSet.length);
    var _newRow = _newRow.replace(/{{id}}/gi, 'checkbox-' + new Date().getTime());
    var ret = true;
    if (isGUI){
        ret = AddRule();
    }
    if (ret === false) {
        console.log("Empty rule addition failed!");
        return;
    }
    console.log("Empty rule addition succeeded!");
    globalRuleSet.push(createTemplate());
    $(".mdl-data-dynamictable tbody tr:last").before(_newRow);
    componentHandler.upgradeAllRegistered();
}

$(".add-row").on("click", function () {
    $(".mdl-dialog__addContent").remove();
    addNewRow();
});
$(".remove-row").on("click", function () {
    $(".mdl-dialog__addContent").remove();
});
$(document).on("click", ".checkbox", function () {
    var _tableRow = $(this).parents("tr:first");
    if ($(this).hasClass("is-checked") === false) {
        _tableRow.addClass("is-selected");
    } else {
        _tableRow.removeClass("is-selected");
    }

});
$(document).on("click", "span.mdl-data-table__label.add-table-content", function () {
    var _modal, _pattern, _error = "";

    $(".mdl-dialog__addContent").remove();
    if ($(this).parents("td:first").hasClass("mdl-data-table__cell--non-numeric") === false) {
        _pattern = 'pattern="-?[0-9]*(\.[0-9]+)?"';
        _error = "Please, add a numeric value.";
    }

    var template = $('#addContentDialogTemplate').html();
    _modal = template.replace(/{{title}}/, $(this).attr("title")).replace(/{{pattern}}/, _pattern).replace(/{{error}}/, _error);
    $(this).parent().prepend(_modal);
    componentHandler.upgradeDom();
    $(".mdl-textfield__input").focus();
});

$(document).on("click", ".close", function () {
    $(this).parents(".mdl-dialog__addContent").remove();
});

$(document).on("keydown", ".mdl-dialog__addContent", function (e) {
    var code = (e.keyCode ? e.keyCode : e.which);
    switch (code) {
        case 13:
            $(".save.mdl-button").click();
            break;
        case 27:
            $(".close.mdl-button").click();
            break;
        default:
    }

});
$(document).on("click", ".save", function () {
    var _textfield = $(this).parents("td").find(".mdl-textfield");
    var _input = $(this).parents("td").find("input");
    if (_textfield.hasClass("is-invalid") === false && $.trim(_input.val()) !== "") {
        var _col = $(this).parents("td");
        _temp = _col.parents("tr");
        $(_col).find("span")[1].innerHTML = _input.val();
        let j, i;
        for (j = 0; j < n; j++){
            let tr_temp = $("tbody").find("tr")[j];
            if (tr_temp === $(_col).parents("tr")[0]) {
                for (i = 1; i <= n; i++) {
                    if ($(_col.parents("tr")).find("td.mdl-data-table__cell--non-numeric")[i] === _col[0]) {
                        console.log("INDEX " + (j) + ", " + (i-1));
                        globalRuleSet[j][titles[i-1]] = _input.val();
                        break
                    }
                }
                break
            }
        }
        console.log("TEST: " + globalRuleSet);
        //check if globalRuleSet[j] does not contain nulls
        $(_col).find(".mdl-dialog__addContent").remove();
        const _index = $(_temp).prevAll().length;
        var check = false;
        for (var key in globalRuleSet[j]) {
            if (globalRuleSet[_index][key] === null) {
                check = true;
                break;
            }
        }
        if (!check) {
            let args = globalRuleSet[j];
            console.log("ARGS: " + args);
            var ret = true
            if(isGUI){
                EditRule(_index, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8])
            }
            if (ret === false) {
                console.log("Rule modification failed!");
                $(_col.parents("tr")).addClass("failed");
            }
            else{
                $(_col.parents("tr")).removeClass("failed");
                console.log("Rule modification succeeded!");
            }
        }
    }
});

$( "table>tbody" ).sortable({
    items: "tr:not(:last)",
    stop: function( event, ui ) {
        let tr = ui.item[0];
        var _index = $(tr).prevAll().length, oldIndex = $(tr).find("span.mdl-data-table__label")[0].innerHTML;
        console.log("Moved " + oldIndex + " to " + _index);
        //move globalRuleSet[oldIndex] to _index
        let temp = globalRuleSet[oldIndex];
        globalRuleSet.splice(oldIndex, 1);
        globalRuleSet.splice(_index, 0, temp);
        if (isGUI){
            SwapRuleTo(oldIndex, _index)
        }
        $(".mdl-data-dynamictable tbody").find('tr').each(function () {
            var _idx = $(this).prevAll().length;
            $(this).find("span.index").text(_idx);
        });
    },
});
