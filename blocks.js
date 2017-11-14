// This is a block definition for projects like roboblocks
// 
// Main Block
Blockly.Blocks['espui'] = {
    category: Facilino.locales.getKey('LANG_CATEGORY_BLOCKS'),
    helpUrl: Facilino.getHelpUrl('espui'),
    tags: ['webinterface'],
    examples: ['lol.bly'],
    init: function() {
        this.appendDummyInput()
            .appendField('ESPUI Title:')
            .appendField(new Blockly.FieldTextInput('string'), 'ui_name');
        this.appendStatementInput('ui_elements')
            .setCheck('ui_element');
        this.setColour("#37d1f9");
        this.setTooltip("Creates a webinterface on the ESP32");
    }
};

Blockly.Arduino['espui'] = function(block) {
    var ui_name = block.getFieldValue('ui_name');
    var ui_elements = Blockly.Arduino.statementToCode(block, 'ui_elements');
    Blockly.Arduino.definitions_['define_wifi_h'] = '#include <WiFi.h>';
    Blockly.Arduino.definitions_['define_espui_h'] = '#include <ESPUI.h>';
    Blockly.Arduino.setups_['setup_espui'] = ui_elements;
    Blockly.Arduino.setups_['setup_espui'] += '\nESPUI.begin("' + ui_name + '");\n';
    return null;
};

//Elements

Blockly.Blocks['espui_button'] = {
    category: Facilino.locales.getKey('LANG_CATEGORY_BLOCKS'),
    helpUrl: Facilino.getHelpUrl('espui'),
    tags: ['webinterface'],
    examples: ['lol.bly'],
    init: function() {
        var colorOptions = [
            ['none', 'COLOR_NONE'],
            ['turquoise', 'COLOR_TURQUOISE'],
            ['emerald', 'COLOR_EMERALD'],
            ['peterriver', 'COLOR_PETERRIVER'],
            ['wet asphalt', 'COLOR_WETASPHALT'],
            ['sunflower', 'COLOR_SUNFLOWER'],
            ['carrot', 'COLOR_CARROT'],
            ['alizarin', 'COLOR_ALIZARIN'],
        ];
        this.appendDummyInput()
            .appendField('ESPUI Button')
            .appendField(new Blockly.FieldTextInput('name'), 'ui_name');
        this.appendDummyInput()
            .appendField('UI Color')
            .appendField(new Blockly.FieldDropdown(colorOptions), 'ui_color');
        this.setColour("#3357c7");
        this.setPreviousStatement(true, 'ui_element');
        this.setNextStatement(true, 'ui_element');
        this.setTooltip('A web interface button');
        this.appendStatementInput('on_down')
            .appendField('Pressed:')
            .setCheck(null);
        this.appendStatementInput('on_up')
            .appendField('Released:')
            .setCheck(null);
    }
};

Blockly.Arduino['espui_button'] = function(block) {
    var ui_name = block.getFieldValue('ui_name');
    var ui_color = block.getFieldValue('ui_color');
    var ui_name_clean = ui_name.replace(' ', '_');
    var on_down = Blockly.Arduino.statementToCode(block, 'on_down');
    var on_up = Blockly.Arduino.statementToCode(block, 'on_up');
    Blockly.Arduino.definitions_['define_ui_button_' + ui_name_clean] =
        'void button_' + ui_name_clean + '(Control c, int type) {\nswitch(type){\ncase B_DOWN:\n' +
        on_down + '\nbreak;\n' +
        'case B_UP:\n' +
        on_up + '\nbreak;\n' +
        '}\n';

    var code = 'ESPUI.button("' + ui_name + '",&button_' + ui_name_clean + ', ' + ui_color + ');\n';
    return code;
};

Blockly.Blocks['espui_label'] = {
    category: Facilino.locales.getKey('LANG_CATEGORY_BLOCKS'),
    helpUrl: Facilino.getHelpUrl('espui'),
    tags: ['webinterface'],
    examples: ['lol.bly'],
    init: function() {
        var colorOptions = [
            ['none', 'COLOR_NONE'],
            ['turquoise', 'COLOR_TURQUOISE'],
            ['emerald', 'COLOR_EMERALD'],
            ['peterriver', 'COLOR_PETERRIVER'],
            ['wet asphalt', 'COLOR_WETASPHALT'],
            ['sunflower', 'COLOR_SUNFLOWER'],
            ['carrot', 'COLOR_CARROT'],
            ['alizarin', 'COLOR_ALIZARIN'],
        ];
        this.appendDummyInput()
            .appendField('ESPUI Label')
            .appendField(new Blockly.FieldTextInput('label'), 'ui_name');
        this.appendDummyInput()
            .appendField('UI Color')
            .appendField(new Blockly.FieldDropdown(colorOptions), 'ui_color');
        this.appendDummyInput()
            .appendField('Initial Value:')
            .appendField(new Blockly.FieldTextInput('value'), 'start_value');
        this.setColour("#3357c7");
        this.setPreviousStatement(true, 'ui_element');
        this.setNextStatement(true, 'ui_element');
        this.setTooltip('A web interface label you can update from your code');
    }
};

Blockly.Arduino['espui_label'] = function(block) {
    var ui_name = block.getFieldValue('ui_name');
    var ui_color = block.getFieldValue('ui_color');
    var ui_name_clean = ui_name.replace(' ', '_');
    var start_value = block.getFieldValue('start_value');

    var code = 'ESPUI.label("' + ui_name + '", ' + ui_color + ', "' + start_value + '");\n';
    return code;
};

Blockly.Blocks['espui_switcher'] = {
    category: Facilino.locales.getKey('LANG_CATEGORY_BLOCKS'),
    helpUrl: Facilino.getHelpUrl('espui'),
    tags: ['webinterface'],
    examples: ['lol.bly'],
    init: function() {
        var colorOptions = [
            ['none', 'COLOR_NONE'],
            ['turquoise', 'COLOR_TURQUOISE'],
            ['emerald', 'COLOR_EMERALD'],
            ['peterriver', 'COLOR_PETERRIVER'],
            ['wet asphalt', 'COLOR_WETASPHALT'],
            ['sunflower', 'COLOR_SUNFLOWER'],
            ['carrot', 'COLOR_CARROT'],
            ['alizarin', 'COLOR_ALIZARIN'],
        ];
        this.appendDummyInput()
            .appendField('ESPUI Switcher')
            .appendField(new Blockly.FieldTextInput('name'), 'ui_name');
        this.appendDummyInput()
            .appendField('UI Color')
            .appendField(new Blockly.FieldDropdown(colorOptions), 'ui_color');
        this.setColour("#3357c7");
        this.setPreviousStatement(true, 'ui_element');
        this.setNextStatement(true, 'ui_element');
        this.setTooltip('A web interface button');
        this.appendStatementInput('on_on')
            .appendField('When swithed on:')
            .setCheck(null);
        this.appendStatementInput('on_off')
            .appendField('When swithed off:')
            .setCheck(null);
    }
};

Blockly.Arduino['espui_switcher'] = function(block) {
    var ui_name = block.getFieldValue('ui_name');
    var ui_color = block.getFieldValue('ui_color');
    var ui_name_clean = ui_name.replace(' ', '_');
    var on_on = Blockly.Arduino.statementToCode(block, 'on_down');
    var on_off = Blockly.Arduino.statementToCode(block, 'on_up');
    Blockly.Arduino.definitions_['define_ui_switcher_' + ui_name_clean] =
        'void switcher_' + ui_name_clean + '(Control c, int type) {\nswitch(type){\ncase S_ACTIVE:\n' +
        on_on + '\nbreak;\n' +
        'case S_INACTIVE:\n' +
        on_off + '\nbreak;\n' +
        '}\n';

    var code = 'ESPUI.switcher("' + ui_name + '",&switcher_' + ui_name_clean + ', ' + ui_color + ', false);\n';
    return code;
};
