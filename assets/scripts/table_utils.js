const titles = ["Name", "Direction", "Source IP", "Destination IP", "Source Port", "Destination Port", "Protocol", "ACK", "Action"];
const n = titles.length;
var loader = document.createElement('input');
loader.type = 'file';
var globalRuleSet = [];
const isGUI=false;


setInterval(function () {
    if (isGUI){
        let update = RetrieveHitCounts();
        console.log("update:" + update);
        let i = 0;
        $("tbody").find("tr").each(function () {
            $(this).find(".hit_count_class").text(update[i]);
            i++;
        });
    }
}, 1000);
function createTemplate(){
    let dict = {};
    //iterate over titles and insert to arrTemplate with a null value
    for(let i = 0; i < titles.length; i++){
        dict[titles[i]] = null;
    }
    return dict
}
function loadJSON(e){
    let _rules = JSON.parse(e);
    //delete all rows except the last one
    $("tbody").find("tr:not(:last)").each(function () {
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

loader.onchange = function (e) {
    var file = e.target.files[0];
    console.log("FILE: " + file);
    //read file as a JSON object
    loadJSON(e.target.result);
};

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
        ret = AddRule(ruleArray[0], ruleArray[1], ruleArray[2], ruleArray[3], ruleArray[4], ruleArray[5], ruleArray[6], ruleArray[7], ruleArray[8]);
    }
    if (ret === false) {
        console.log("Ready rule addition failed!");
    }
    else{
        console.log("Ready rule addition succeeded!");
    }
    if (!check && ret) {
        //remove "failed" class from _newRow
        _newRow = _newRow.replace(/failed/gi, "");
    }
    $(".mdl-data-dynamictable tbody tr:last").before(_newRow);
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
function ruleModify(_input, _col){
    let _temp = _col.parents("tr");
    let j, i;
    let flag = false;
    for (j = 0; j < n; j++){
        let tr_temp = $("tbody").find("tr")[j];
        if (tr_temp === $(_col).parents("tr")[0]) {
            for (i = 1; i <= n; i++) {
                if ($(_col.parents("tr")).find("td.mdl-data-table__cell--non-numeric")[i] === _col[0]) {
                    console.log("INDEX " + (j) + ", " + (i-1));
                    var ret =  checkValidity(_input.val(), i-1, j);
                    if (ret === false) {
                        _col.addClass("failed");
                    }
                    else {
                        _col.removeClass("failed");
                        globalRuleSet[j][titles[i-1]] = _input.val();
                        flag = true;
                    }
                    break
                }
            }
            break
        }
    }
    if (!flag) {
        return;
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
    if (!check){
        let args = globalRuleSet[j];
        //print the args to the console, but make it readable and not [Object object]
        console.log("ARGS: " + JSON.stringify(args).replace(/\[|\]/g, "").replace(/\,/g, ", "));
        ret = true
        if(isGUI){
            ret = EditRule(_index,  args[titles[0]], args[titles[1]], args[titles[2]], args[titles[3]], args[titles[4]], args[titles[5]], args[titles[6]], args[titles[7]], args[titles[8]]);
        }
        if (ret === false) {
            console.log("Rule modification failed!");
        }
        else{
            console.log("Rule modification succeeded!");
        }
    }
}
//when select is changed, update the rule
$(document).on("change", "select", function () {
    //get selected value
    var _input = $(this);
    var _col = $(this).parents("td");
    ruleModify(_input, _col);
});

$(document).on("click", ".save", function () {
    var _textfield = $(this).parents("td").find(".mdl-textfield");
    var _input = $(this).parents("td").find("input");
    if (_textfield.hasClass("is-invalid") === false && $.trim(_input.val()) !== "") {
        var _col = $(this).parents("td");
        $(_col).find("span")[1].innerHTML = _input.val();
        ruleModify(_input, _col);
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

function checkValidity(text, index, currentRow){
    //indices meaning:
//0: rule name ( make sure no valid existing rule name is using the same name)
//1: rule direction  (in or out)
    //2: make sure this is a valid ip address
    // 3: make sure this is a valid ip address
    // 4: make sure this is a valid port
    // 5: make sure this is a valid port
    // 6: make sure this is a valid protocol type (TCP or UDP)
    // 7: make sure this is a valid ack bit (yes or no)
    // 8: make sure this is a valid action (allow or drop)
    switch (index){
        case 0:
            if (text === "") {
                angular.element(document.getElementById('table_scope')).scope().change("Name is empty!", true);
                return false;
            }
            if(text === "default"){
                angular.element(document.getElementById('table_scope')).scope().change("Name cannot be default!", true);
                return false;
            }
            for (var i = 0; i < globalRuleSet.length; i++) {
                if (globalRuleSet[i][titles[0]] === text && i !== currentRow) {
                    angular.element(document.getElementById('table_scope')).scope().change("Name already exists!", true);
                    return false;
                }
            }
            return true;
        case 1:
            return text !== "";

        case 2:
            if(text.toLowerCase() === "any"){
                return true;
            }
            return validateIP(text, "Source");
        case 3:
            if(text.toLowerCase() === "any"){
                return true;
            }
            return validateIP(text, "Destination");
        case 4:
            if(text.toLowerCase() === "any"){
                return true;
            }
            return validatePort(text, "Source");
        case 5:
            if(text.toLowerCase() === "any"){
                return true;
            }
            return validatePort(text, "Destination");
        case 6:
            return text !== "";
        case 7:
            return text !== "";
        case 8:
            return text !== "";
        default:
            return false;
    }
}
function validateCIDR(cidr){
    angular.element(document.getElementById('table_scope')).scope().change("CIDR not in [0,32] range!", true);
    return cidr >= 0 && cidr <= 32;
}
function validateIP(text, meaning){
    //allow also CIDR notation while also checking if it is a valid ip address
    if (text.includes("/")) {
        var _temp = text.split("/");
        if (_temp.length !== 2) {
            angular.element(document.getElementById('table_scope')).scope().change("Invalid CIDR usage", true);
            return false;
        }
        else {
            return validateIP(_temp[0], meaning) && validateCIDR(_temp[1]);
        }
    }
    const split = text.split(".");
    if (split.length !== 4) {
        angular.element(document.getElementById('table_scope')).scope().change(meaning + " IP must be of type x.x.x.x", true);
        return false;
    }
    for (var i = 0; i < split.length; i++) {
        //check if split[i] is a number
        if (isNaN(split[i])) {
            angular.element(document.getElementById('table_scope')).scope().change(meaning + " IP bytes must only include numbers", true);
            return false;
        }
        if (split[i] < 0 || split[i] > 255) {
            let temp;
            if (i === 0) {
                temp = "First";
            }
            else if (i === 1) {
                temp = "Second";
            }
            else if (i === 2) {
                temp = "Third";
            }
            else if (i === 3) {
                temp = "Fourth";
            }
            temp +=  " byte of " + meaning + " IP must be of type 0-255";
            angular.element(document.getElementById('table_scope')).scope().change(temp , true);
            return false;
        }
    }
    return true;
}

function validatePort(text, meaning){
    //check if it's a range first

    if (text.includes("-") && text.length > 2) {
        var _temp = text.split("-");
        if (_temp.length !== 2) {
            angular.element(document.getElementById('table_scope')).scope().change("Invalid " + meaning + " port range", true);
            return false;
        }
        else {
            return validatePort(_temp[0], meaning) && validatePort(_temp[1]);
        }
    }
    //check if it's a single port
    if(text >= 0 && text <= 65535 && !isNaN(text)){
        return true;
    }
    angular.element(document.getElementById('table_scope')).scope().change("Ports should be in [0,65535]", true);
    return false;

}

