angular.module('modalTest', ['ui.bootstrap', 'dialogs'])
    .controller('dialogServiceTest', function ($scope, $rootScope, $timeout, $dialogs) {
        $scope.launch = function () {
            var dlg = null;
            dlg = $dialogs.confirm('Please Confirm', 'Are you sure you want to deleted selected elements?');
            dlg.result.then(function (btn) {
                $(".mdl-data-dynamictable tbody").find('tr.is-selected').remove();
                $(".mdl-data-dynamictable thead tr").removeClass("is-selected");
                $(".mdl-data-dynamictable thead tr th label").removeClass("is-checked");
                componentHandler.upgradeDom();
                var _row = $(".mdl-data-dynamictable tbody").find('tr');
                console.log("_row.length", _row.length);
                if (_row.length < 1) {
                    addNewRow();
                }
            }, function (btn) {
            });
        }; // end launch

    }); // end run / module

function addNewRow() {

    var _row = $(".mdl-data-dynamictable tbody").find('tr');
    var template = $('#basketItemTemplate').html();
    var _newRow = template.replace(/{{id}}/gi, 'checkbox-' + new Date().getTime());
    $(".mdl-data-dynamictable tbody tr:last").before(_newRow);
    componentHandler.upgradeAllRegistered();
}

var _isinvalid = false;
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
//TODO:This is buggy if you select all and decide to deselect a few, maybe use angular checkbox code Tom wrote for MadSec..
/*$(document).on("click", "#checkbox-all", function() {
    _isChecked = $(this).parent("label").hasClass("is-checked");
    if (_isChecked === false) {
        $(".mdl-data-dynamictable").find('tr').not('.checkbox-default').addClass("is-selected");
        $(".mdl-data-dynamictable").find('tr').not('.checkbox-default').find('td label').addClass("is-checked");
    } else {
        $(".mdl-data-dynamictable").find('tr').not('.checkbox-default').removeClass("is-selected");
        $(".mdl-data-dynamictable").find('tr').not('.checkbox-default').find('td label').removeClass("is-checked");
    }

});*/
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
var globalRuleSet = [];
$(document).on("click", ".save", function () {
    var _textfield = $(this).parents("td").find(".mdl-textfield");
    var _input = $(this).parents("td").find("input");
    if (_textfield.hasClass("is-invalid") === false && $.trim(_input.val()) !== "") {
        var _col = $(this).parents("td");
        _temp = _col.parents("tr");
        $(_col).find("span")[1].innerHTML = _input.val();
        $(_col).find(".mdl-dialog__addContent").remove();
        if (parseInt(_col.attr("data-on"), 2) === 0) {
            _col.parent("tr").attr("data-count", parseInt(_col.parent("tr").attr("data-count"), 10) + 1);
            _col.attr("data-on", 1);
        }
        const _index = $(_temp).prevAll().length;
        const n = 9;
        if (parseInt(_col.parent("tr").attr("data-count"), 10) === 9) {
            var args = [];
            for (let i = 0; i < n; ++i) {
                console.log(i);
                args.push($(_col.parents("tr")).find("span.mdl-data-table__label")[i].innerHTML);
            }
            console.log(args);
            if (globalRuleSet.length === _index) {
                var ret = AddRule(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8])
                if (ret === false) {
                    console.log("Rule addition failed!");
                    $(_col.parents("tr")).addClass("failed");
                }
                else{
                    $(_col.parents("tr")).removeClass("failed");
                    console.log("Rule addition succeeded!");
                }
                globalRuleSet.push(args);
            } else {
                globalRuleSet[_index] = args;
                var ret = EditRule(_index, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8])
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
    }
});