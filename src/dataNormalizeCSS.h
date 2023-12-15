const char CSS_NORMALIZE[] PROGMEM = R"=====(
html{font-family:sans-serif;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}body{margin:0}article,aside,details,figcaption,figure,footer,header,hgroup,main,menu,nav,section,summary{display:block}audio,canvas,progress,video{display:inline-block;vertical-align:baseline}audio:not([controls]){display:none;height:0}[hidden],template{display:none}a{background-color:transparent}a:active,a:hover{outline:0}abbr[title]{border-bottom:1px dotted}b,strong{font-weight:bold}dfn{font-style:italic}h1{font-size:2em;margin:.67em 0}mark{background:#ff0;color:#000}small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:baseline}sup{top:-.5em}sub{bottom:-.25em}img{border:0}svg:not(:root){overflow:visible}figure{margin:1em 40px}hr{-moz-box-sizing:content-box;box-sizing:content-box;height:0}pre{overflow:auto}code,kbd,pre,samp{font-family:monospace,monospace;font-size:1em}button,input,optgroup,select,textarea{color:inherit;font:inherit;margin:0}button{overflow:visible}button,select{text-transform:none}button,html input[type="button"],input[type="reset"],input[type="submit"]{-webkit-appearance:button;cursor:pointer}button[disabled],html input[disabled]{cursor:default}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}input{line-height:normal}input[type="checkbox"],input[type="radio"]{box-sizing:border-box;padding:0}input[type="number"]::-webkit-inner-spin-button,input[type="number"]::-webkit-outer-spin-button{height:auto}input[type="search"]{-webkit-appearance:textfield;-moz-box-sizing:content-box;-webkit-box-sizing:content-box;box-sizing:content-box}input[type="search"]::-webkit-search-cancel-button,input[type="search"]::-webkit-search-decoration{-webkit-appearance:none}fieldset{border:1px solid silver;margin:0 2px;padding:.35em .625em .75em}legend{border:0;padding:0}textarea{overflow:auto}optgroup{font-weight:bold}table{border-collapse:collapse;border-spacing:0}td,th{padding:0}
)=====";

const uint8_t CSS_NORMALIZE_GZIP[859] PROGMEM = { 31,139,8,0,126,105,124,101,2,255,149,84,237,110,163,58,16,125,149,168,171,74,187,146,137,104,247,163,87,70,247,73,162,252,24,236,1,124,227,47,217,38,77,22,241,238,59,6,66,147,54,93,233,254,2,6,123,230,204,57,103,166,75,70,15,141,179,169,104,192,40,125,230,17,108,44,34,6,213,84,133,137,69,194,83,42,162,250,141,5,200,255,250,152,248,83,89,62,86,197,43,214,7,149,238,255,29,107,39,207,131,129,208,42,203,203,17,66,82,66,35,131,168,36,50,137,9,148,142,172,81,173,0,159,148,179,249,181,15,200,26,231,18,6,214,33,200,252,104,131,235,61,51,160,44,51,104,123,102,225,200,34,138,233,70,236,13,165,63,15,82,69,175,225,204,107,237,196,97,132,94,42,199,4,216,35,68,230,131,107,3,198,200,142,84,213,173,39,149,213,202,98,49,93,168,142,152,161,129,46,64,171,214,242,26,34,230,191,115,34,110,93,250,186,19,196,76,112,58,238,191,173,41,172,179,88,117,168,218,46,81,119,187,78,73,137,118,207,18,26,250,157,240,230,220,8,67,13,226,144,123,177,178,16,78,187,192,83,32,134,61,4,180,105,4,14,212,209,145,200,225,157,35,56,131,235,83,134,144,105,171,235,176,75,42,105,220,15,181,11,196,73,81,187,148,156,225,79,254,180,145,244,138,114,172,89,36,120,182,157,21,124,157,65,213,78,203,81,54,118,14,198,116,214,200,85,162,30,197,216,61,45,65,146,140,63,163,169,22,149,182,191,94,208,108,202,145,62,15,87,136,249,151,166,41,171,25,246,151,178,44,199,104,64,235,171,20,255,144,218,177,39,20,189,191,138,190,252,124,172,38,154,47,44,85,222,69,149,149,227,1,137,35,106,248,83,238,115,166,228,60,47,182,63,209,228,220,195,210,117,177,125,206,17,101,218,133,14,226,40,30,219,73,38,30,200,59,223,134,204,96,163,221,43,63,170,168,106,141,227,236,172,139,21,159,168,197,31,165,63,141,93,24,10,227,126,19,159,167,12,88,217,150,103,157,73,144,28,170,62,9,175,146,123,74,185,150,130,62,185,81,56,114,246,161,150,228,58,100,17,140,191,153,40,227,172,35,193,5,178,245,173,122,35,139,80,141,117,79,45,90,166,172,239,19,115,62,205,222,39,70,200,239,44,207,24,185,5,134,89,7,101,59,26,206,52,101,88,63,214,97,155,51,125,100,98,169,48,167,28,166,177,157,124,216,184,96,102,167,46,39,58,218,7,155,9,200,46,157,61,254,251,48,199,31,246,236,58,72,147,133,233,93,140,180,50,138,130,195,101,57,128,247,8,84,68,32,159,147,84,162,15,145,90,240,78,17,173,97,41,185,163,129,1,194,40,247,215,197,215,224,176,92,146,216,64,175,211,114,137,243,73,193,198,137,62,22,202,90,218,24,211,189,143,241,213,45,149,7,41,179,168,229,56,29,29,174,45,106,137,7,208,227,117,63,162,67,113,32,225,223,183,14,180,28,30,242,76,174,46,89,199,243,244,190,198,114,199,246,166,198,240,176,39,116,11,55,19,180,34,122,101,139,107,241,63,61,79,123,225,246,252,176,0,159,252,119,35,3,113,46,186,251,50,100,221,27,133,90,86,127,243,255,229,226,255,26,143,187,24,222,240,207,145,66,100,24,250,94,203,159,94,145,40,92,128,188,60,238,117,52,89,119,106,137,12,121,145,58,47,200,232,180,146,155,168,52,77,194,58,30,155,103,255,38,209,246,59,237,147,205,246,215,243,244,120,201,203,69,99,139,86,222,115,204,58,132,183,131,127,153,213,143,251,55,101,247,94,22,55,77,174,6,31,145,95,94,170,229,71,94,6,75,1,201,82,55,188,21,252,3,26,45,63,124,152,7,0,0 };
