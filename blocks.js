// This is a block definition for projects like roboblocks
//
// Main Block
Blockly.Blocks['espui'] = {
    category: Facilino.locales.getKey('LANG_CATEGORY_BLOCKS'),
    helpUrl: Facilino.getHelpUrl('espui'),
    tags: ['webinterface'],
    examples: ['lol.bly'],
    init: function() {
        var wifiOptions = [
            ['No', false],
            ['Yes', true]
          ];
        this.appendDummyInput()
            .appendField('ESPUI Title:')
            .appendField(new Blockly.FieldTextInput('string'), 'ui_name');
       this.appendDummyInput()
            .appendField('Enable Wifi Hotspot Code:')
            .appendField(new Blockly.FieldDropdown(wifiOptions), 'wifi_option');
        this.appendStatementInput('ui_elements')
            .setCheck('ui_element');
        this.setColour("#37d1f9");
        this.setTooltip("Creates a webinterface on the ESP32");
    }
};

Blockly.Arduino['espui'] = function(block) {
    var ui_name = block.getFieldValue('ui_name');
    var wifi_option = block.getFieldValue('wifi_option');
    var ui_elements = Blockly.Arduino.statementToCode(block, 'ui_elements');
    Blockly.Arduino.definitions_['define_wifi_h'] = '#include <WiFi.h>';
    Blockly.Arduino.definitions_['define_espui_h'] = '#include <ESPUI.h>';
    Blockly.Arduino.setups_['setup_espui'] = '\n';
    if(wifi_option){
      Blockly.Arduino.setups_['setup_espui'] +=
      '    Serial.begin(115200);\n\n' +
      '    WiFi.mode(WIFI_AP);\n' +
      '    WiFi.softAP("' + ui_name + '");\n' +
      '    Serial.print("IP address: ");\n' +
      '    Serial.println(WiFi.softAPIP());\n\n';
    }
    Blockly.Arduino.setups_['setup_espui'] += ui_elements;
    Blockly.Arduino.setups_['setup_espui'] += '    ESPUI.begin("' + ui_name + '");\n';
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
        this.appendDummyInput()
            .appendField('Button Text')
            .appendField(new Blockly.FieldTextInput('name'), 'button_text');
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
    var button_text = block.getFieldValue('button_text');
    var ui_name_clean = ui_name.replace(' ', '_');
    var on_down = Blockly.Arduino.statementToCode(block, 'on_down');
    var on_up = Blockly.Arduino.statementToCode(block, 'on_up');
    Blockly.Arduino.definitions_['define_ui_button_' + ui_name_clean] =
        'void button_' + ui_name_clean + '(Control c, int type) {\n' +
        ' switch(type){\n' +
        '  case B_DOWN:\n' +
        on_down + '\n    break;\n' +
        '  case B_UP:\n' +
        on_up + '\n    break;\n' +
        ' }\n' +
        '}\n';

    var code = '  ESPUI.button("' + ui_name + '", &button_' + ui_name_clean + ', ' + ui_color + ', "' + button_text + '");\n';
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

    var code = '  ESPUI.label("' + ui_name + '", ' + ui_color + ', "' + start_value + '");\n';
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
        var stateOptions = [
            ['Off', 'false'],
            ['On', 'true'],
        ];
        this.appendDummyInput()
            .appendField('ESPUI Switcher')
            .appendField(new Blockly.FieldTextInput('name'), 'ui_name');
        this.appendDummyInput()
            .appendField('UI Color')
            .appendField(new Blockly.FieldDropdown(colorOptions), 'ui_color');
        this.appendDummyInput()
            .appendField('Initial state')
            .appendField(new Blockly.FieldDropdown(stateOptions), 'switcher_state');
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
    var switcher_state = block.getFieldValue('switcher_state');
    var ui_name_clean = ui_name.replace(' ', '_');
    var on_on = Blockly.Arduino.statementToCode(block, 'on_down');
    var on_off = Blockly.Arduino.statementToCode(block, 'on_up');
    Blockly.Arduino.definitions_['define_ui_switcher_' + ui_name_clean] =
        'void switcher_' + ui_name_clean + '(Control c, int type) {\n' +
        ' switch(type){\n' +
        '  case S_ACTIVE:\n' +
        on_on + '\n    break;\n' +
        '  case S_INACTIVE:\n' +
        on_off + '\n    break;\n' +
        ' }\n' +
        '}\n';

    var code = '  ESPUI.switcher("' + ui_name + '", ' + switcher_state + ', &switcher_' + ui_name_clean + ', ' + ui_color + ');\n';
    return code;
};

Blockly.Blocks['espui_pad'] = {
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
        var centerOptions = [
            ['Yes', 'false'],
            ['No', 'true'],
        ];
        this.appendDummyInput()
            .appendField('ESPUI ButtonPad')
            .appendField(new Blockly.FieldTextInput('name'), 'ui_name');
        this.appendDummyInput()
            .appendField('UI Color')
            .appendField(new Blockly.FieldDropdown(colorOptions), 'ui_color');
        this.appendDummyInput()
            .appendField('Center button')
            .appendField(new Blockly.FieldDropdown(centerOptions), 'pad_center');
        this.setColour("#3357c7");
        this.setPreviousStatement(true, 'ui_element');
        this.setNextStatement(true, 'ui_element');
        this.setTooltip('A web interface button');

        this.appendStatementInput('on_down_for')
            .appendField('Forward press:')
            .setCheck(null);
        this.appendStatementInput('on_up_for')
            .appendField('Forward release:')
            .setCheck(null);

        this.appendStatementInput('on_down_back')
            .appendField('Back press:')
            .setCheck(null);
        this.appendStatementInput('on_up_back')
            .appendField('Back release:')
            .setCheck(null);

        this.appendStatementInput('on_down_left')
            .appendField('Right press:')
            .setCheck(null);
        this.appendStatementInput('on_up_left')
            .appendField('Right release:')
            .setCheck(null);

        this.appendStatementInput('on_down_right')
            .appendField('Left press:')
            .setCheck(null);
        this.appendStatementInput('on_up_right')
            .appendField('Left release:')
            .setCheck(null);

        this.appendStatementInput('on_down_center')
            .appendField('Center press:')
            .setCheck(null);
        this.appendStatementInput('on_up_center')
            .appendField('Center release:')
            .setCheck(null);
    }
};

Blockly.Arduino['espui_pad'] = function(block) {
    var ui_name = block.getFieldValue('ui_name');
    var ui_color = block.getFieldValue('ui_color');
    var pad_center = block.getFieldValue('pad_center');
    var ui_name_clean = ui_name.replace(' ', '_');

    var on_down_for = Blockly.Arduino.statementToCode(block, 'on_down_for');
    var on_up_for = Blockly.Arduino.statementToCode(block, 'on_up_for');

    var on_down_back = Blockly.Arduino.statementToCode(block, 'on_down_back');
    var on_up_back = Blockly.Arduino.statementToCode(block, 'on_up_back');

    var on_down_left = Blockly.Arduino.statementToCode(block, 'on_down_left');
    var on_up_left = Blockly.Arduino.statementToCode(block, 'on_up_left');

    var on_down_right = Blockly.Arduino.statementToCode(block, 'on_down_right');
    var on_up_right = Blockly.Arduino.statementToCode(block, 'on_up_right');

    var on_down_center = Blockly.Arduino.statementToCode(block, 'on_down_center');
    var on_up_center = Blockly.Arduino.statementToCode(block, 'on_up_center');


    Blockly.Arduino.definitions_['define_ui_pad_' + ui_name_clean] =
        'void pad_' + ui_name_clean + '(Control c, int type) {\n' +
        ' switch(type){\n' +

        '   case P_FOR_DOWN:\n' +
        on_down_for + '\n    break;\n' +
        '   case P_FOR_UP:\n' +
        on_up_for + '\n    break;\n' +

        '   case P_BACK_DOWN:\n' +
        on_down_back + '\n    break;\n' +
        '   case P_BACK_UP:\n' +
        on_up_back + '\n   break;\n' +

        '   case P_RIGHT_DOWN:\n' +
        on_down_left + '\n    break;\n' +
        '   case P_RIGHT_UP:\n' +
        on_up_left + '\n    break;\n' +

        '  case P_LEFT_DOWN:\n' +
        on_down_right + '\n    break;\n' +
        '  case P_LEFT_UP:\n' +
        on_up_right + '\n    break;\n' +

        '  case P_CENTER_DOWN:\n' +
        on_down_center + '\n    break;\n' +
        '  case P_CENTER_UP:\n' +
        on_up_center + '\n    break;\n' +
        ' }\n' +
        '}\n';

    var code = '  ESPUI.pad("' + ui_name + '", ' + pad_center + ', &pad_' + ui_name_clean + ', ' + ui_color + ');\n';
    return code;
};
