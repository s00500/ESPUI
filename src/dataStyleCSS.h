const char CSS_STYLE[] PROGMEM = R"=====(
.container{position:relative;width:79%;margin:20px;box-sizing:border-box}.column,.columns{width:100%;float:left}.card{min-height:100px;border-radius:6px;box-shadow:0 4px 4px rgba(204,197,185,0.5);padding-left:20px;padding-right:20px;margin-bottom:40px;min-width:500px;color:#fff}@media(min-width:1205px){.wide.card{min-width:1075px}}@media(min-width:1790px){.wide.card{min-width:1650px}}@media(max-width:630px){.card{min-width:98%}}.sectionbreak.columns{color:black}.sectionbreak.columns hr{border:0;height:2px;background-color:#666}.turquoise{background:#1abc9c;border-bottom:#16a085 3px solid}.emerald{background:#2ecc71;border-bottom:#27ae60 3px solid}.peterriver{background:#3498db;border-bottom:#2980b9 3px solid}.wetasphalt{background:#34495e;border-bottom:#2c3e50 3px solid}.sunflower{background:#f1c40f;border-bottom:#e6bb0f 3px solid}.carrot{background:#e67e22;border-bottom:#d35400 3px solid}.alizarin{background:#e74c3c;border-bottom:#c0392b 3px solid}.dark{background:#444857;border-bottom:#444857 3px solid}.label{box-sizing:border-box;white-space:nowrap;border-radius:.2em;padding:.12em .4em .14em;text-align:center;color:#fff;font-weight:700;line-height:1.3;margin-bottom:5px;display:inline-block;white-space:nowrap;vertical-align:baseline;position:relative;top:-.15em;background-color:#999;margin-bottom:10px}.label-wrap{width:90%;white-space:pre-wrap;word-wrap:break-word}.label.color-blue{background-color:#6f9ad1}.label.color-red{background-color:#d37c7c}.label.color-green{background-color:#9bc268}.label.color-orange{background-color:#dea154}.label.color-yellow{background-color:#e9d641}.label.color-purple{background-color:#9f83d1}@media(min-width:400px){.container{width:84%}}@media(min-width:630px){.container{width:98%}.column,.columns{margin-right:35px}.column:first-child,.columns:first-child{margin-left:0}.one.column,.one.columns{width:4.66666666667%}.two.columns{width:13.3333333333%}.three.columns{width:22%}.four.columns{width:30.6666666667%}.five.columns{width:39.3333333333%}.six.columns{width:48%}.seven.columns{width:56.6666666667%}.eight.columns{width:65.3333333333%}.nine.columns{width:74%}.ten.columns{width:82.6666666667%}.eleven.columns{width:91.3333333333%}.twelve.columns{width:100%;margin-left:0}.one-third.column{width:30.6666666667%}.two-thirds.column{width:65.3333333333%}.one-half.column{width:48%}.offset-by-one.column,.offset-by-one.columns{margin-left:8.66666666667%}.offset-by-two.column,.offset-by-two.columns{margin-left:17.3333333333%}.offset-by-three.column,.offset-by-three.columns{margin-left:26%}.offset-by-four.column,.offset-by-four.columns{margin-left:34.6666666667%}.offset-by-five.column,.offset-by-five.columns{margin-left:43.3333333333%}.offset-by-six.column,.offset-by-six.columns{margin-left:52%}.offset-by-seven.column,.offset-by-seven.columns{margin-left:60.6666666667%}.offset-by-eight.column,.offset-by-eight.columns{margin-left:69.3333333333%}.offset-by-nine.column,.offset-by-nine.columns{margin-left:78%}.offset-by-ten.column,.offset-by-ten.columns{margin-left:86.6666666667%}.offset-by-eleven.column,.offset-by-eleven.columns{margin-left:95.3333333333%}.offset-by-one-third.column,.offset-by-one-third.columns{margin-left:34.6666666667%}.offset-by-two-thirds.column,.offset-by-two-thirds.columns{margin-left:69.3333333333%}.offset-by-one-half.column,.offset-by-one-half.columns{margin-left:52%}}html{font-size:62.5%}body{margin:0;font-size:1.5em;line-height:1;font-weight:400;font-family:"Open Sans",sans-serif;color:#222;background-color:#ecf0f1}h1,h2,h3,h4,h5,h6{margin-top:0;margin-bottom:.5rem;font-weight:300}h1{font-size:4rem;line-height:1.2;letter-spacing:-.1rem}h2{font-size:3.6rem;line-height:1.25;letter-spacing:-.1rem}h3{font-size:3rem;line-height:1.3;letter-spacing:-.1rem}h4{font-size:2.4rem;line-height:1.35;letter-spacing:-.08rem}h5{font-size:1.8rem;line-height:1.5;letter-spacing:-.05rem}h6{font-size:1.5rem;line-height:1.6;letter-spacing:0}@media(min-width:630px){h1{font-size:5rem}h2{font-size:4.2rem}h3{font-size:3.6rem}h4{font-size:3rem}h5{font-size:2rem}h6{font-size:1.5rem}}p{margin-top:0}a{color:#1eaedb}a:hover{color:#0fa0ce}button{display:inline-block;padding:10px;border-radius:3px;color:#fff;background-color:#999}button:enabled:active{background-color:#666;transform:translateX(4px) translateY(4px)}#mainHeader{display:inline-block}#conStatus{position:inherit;font-size:.75em}button,.button{margin-bottom:1rem;margin-left:.3rem;margin-right:.3rem}.u-full-width{width:100%;box-sizing:border-box}.u-max-full-width{max-width:100%;box-sizing:border-box}.u-pull-right{float:right}.u-pull-left{float:left}.tcenter{text-align:center}hr{margin-top:.5rem;margin-bottom:1.2rem;border-width:0;border-top:1px solid #e1e1e1}.container:after,.row:after,.u-cf{content:"";display:table;clear:both}.control{background-color:#ddd;background-image:linear-gradient(hsla(0,0%,0%,0.1),hsla(0,0%,100%,0.1));border-radius:50%;box-shadow:inset 0 1px 1px 1px hsla(0,0%,100%,0.5),0 0 1px 1px hsla(0,0%,100%,0.75),0 0 1px 2px hsla(0,0%,100%,0.25),0 0 1px 3px hsla(0,0%,100%,0.25),0 0 1px 4px hsla(0,0%,100%,0.25),0 0 1px 6px hsla(0,0%,0%,0.75);height:9em;margin:3em auto;position:relative;width:9em}.control ul{height:100%;padding:0;transform:rotate(45deg)}.control li{border-radius:100% 0 0 0;box-shadow:inset -1px -1px 1px hsla(0,0%,100%,0.5),0 0 1px hsla(0,0%,0%,0.75);display:inline-block;height:50%;overflow:hidden;width:50%}.control ul li:nth-child(2){transform:rotate(90deg)}.control ul li:nth-child(3){transform:rotate(-90deg)}.control ul li:nth-child(4){transform:rotate(180deg)}.control ul a{height:200%;position:relative;transform:rotate(-45deg);width:200%}.control a:hover,.control a:focus{background-color:hsla(0,0%,100%,0.25)}.control a{border-radius:50%;color:#333;display:block;font:bold 1em/3 sans-serif;text-align:center;text-decoration:none;text-shadow:0 1px 1px hsla(0,0%,100%,0.4);transition:.15s}.control .confirm{background-color:#ddd;background-image:linear-gradient(hsla(0,0%,0%,0.15),hsla(0,0%,100%,0.25));box-shadow:inset 0 1px 1px 1px hsla(0,0%,100%,0.5),0 0 1px 1px hsla(0,0%,100%,0.25),0 0 1px 2px hsla(0,0%,100%,0.25),0 0 1px 3px hsla(0,0%,100%,0.25),0 0 1px 4px hsla(0,0%,100%,0.25),0 0 1px 6px hsla(0,0%,0%,0.85);left:50%;line-height:3;margin:-1.5em;position:absolute;top:50%;width:3em}.control .confirm:hover,.control .confirm:focus{background-color:#eee}.control:not(.disabled) a.confirm:active{background-color:#777}.control:not(.disabled) li:active{background-color:#777}.switch{display:inline-block !important;background-color:#bebebe;border-radius:4px;box-shadow:inset 0 0 6px rgba(0,0,0,0.3);color:#fff;cursor:pointer;display:block;font-size:14px;height:26px;margin-left:.3rem;margin-right:.3rem;position:relative;width:60px;-webkit-transition:background-color .2s ease-in-out;-moz-transition:background-color .2s ease-in-out;-o-transition:background-color .2s ease-in-out;-ms-transition:background-color .2s ease-in-out;transition:background-color .2s ease-in-out}.switch.checked{background-color:#76d21d}.switch input[type="checkbox"]{display:none;cursor:pointer;height:10px;left:12px;position:absolute;top:8px;width:10px}.in{position:absolute;top:8px;left:12px;-webkit-transition:left .08s ease-in-out;-moz-transition:left .08s ease-in-out;-o-transition:left .08s ease-in-out;-ms-transition:left .08s ease-in-out;transition:left .08s ease-in-out}.switch.checked div{left:38px}.switch .in:before{background:#fff;background:-moz-linear-gradient(top,#fff 0,#f0f0f0 100%);background:-webkit-gradient(linear,left top,left bottom,color-stop(0,#fff),color-stop(100%,#f0f0f0));background:-webkit-linear-gradient(top,#fff 0,#f0f0f0 100%);background:-o-linear-gradient(top,#fff 0,#f0f0f0 100%);background:-ms-linear-gradient(top,#fff 0,#f0f0f0 100%);background:linear-gradient(to bottom,#fff 0,#f0f0f0 100%);border:1px solid #fff;border-radius:2px;box-shadow:0 0 4px rgba(0,0,0,0.3);content:"";height:18px;position:absolute;top:-5px;left:-9px;width:26px}.switch .in:after{background:#f0f0f0;background:-moz-linear-gradient(top,#f0f0f0 0,#fff 100%);background:-webkit-gradient(linear,left top,left bottom,color-stop(0,#f0f0f0),color-stop(100%,#fff));background:-webkit-linear-gradient(top,#f0f0f0 0,#fff 100%);background:-o-linear-gradient(top,#f0f0f0 0,#fff 100%);background:-ms-linear-gradient(top,#f0f0f0 0,#fff 100%);background:linear-gradient(to bottom,#f0f0f0 0,#fff 100%);border-radius:10px;content:"";height:12px;margin:-1px 0 0 -1px;position:absolute;width:12px}.rkmd-slider{display:block;position:relative;font-size:16px;font-family:"Roboto",sans-serif}.rkmd-slider input[type="range"]{overflow:hidden;position:absolute;width:1px;height:1px;opacity:0}.rkmd-slider input[type="range"]+.slider{display:block;position:relative;width:100%;height:27px;border-radius:13px;background-color:#bebebe}@media(pointer:fine){.rkmd-slider input[type="range"]+.slider{height:4px;border-radius:0}}.rkmd-slider input[type="range"]+.slider .slider-fill{display:block;position:absolute;width:0;height:100%;user-select:none;z-index:1}.rkmd-slider input[type="range"]+.slider .slider-handle{cursor:pointer;position:absolute;top:12px;left:0;width:15px;height:15px;margin-left:-8px;border-radius:50%;transition:all .2s ease;user-select:none;z-index:2}@media(pointer:fine){.rkmd-slider input[type="range"]+.slider .slider-handle{top:-5.5px}}.rkmd-slider input[type="range"]:disabled+.slider{background-color:#b0b0b0 !important}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle{cursor:default !important;background-color:#b0b0b0 !important}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill .slider-label,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle .slider-label{display:none;background-color:#b0b0b0 !important}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill.is-active,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle.is-active{top:-5.5px;width:15px;height:15px;margin-left:-8px}.rkmd-slider input[type="range"]:disabled+.slider .slider-fill.is-active .slider-label,.rkmd-slider input[type="range"]:disabled+.slider .slider-handle.is-active .slider-label{display:none;border-radius:50%;transform:none}.rkmd-slider input[type="range"]:disabled+.slider .slider-handle:active{box-shadow:none !important;transform:scale(1) !important}.rkmd-slider.slider-discrete .slider .slider-handle{position:relative;z-index:1}.rkmd-slider.slider-discrete .slider .slider-handle .slider-label{position:absolute;top:-17.5px;left:4px;width:30px;height:30px;-webkit-transform-origin:50% 100%;transform-origin:50% 100%;border-radius:50%;-webkit-transform:scale(1) rotate(-45deg);transform:scale(1) rotate(-45deg);-webkit-transition:all .2s ease;transition:all .2s ease}@media(pointer:fine){.rkmd-slider.slider-discrete .slider .slider-handle .slider-label{left:-2px;-webkit-transform:scale(0.5) rotate(-45deg);transform:scale(0.5) rotate(-45deg)}}.rkmd-slider.slider-discrete .slider .slider-handle .slider-label span{position:absolute;top:7px;left:0;width:100%;color:#fff;font-size:16px;text-align:center;-webkit-transform:rotate(45deg);transform:rotate(45deg);opacity:0;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}@media(pointer:fine){.rkmd-slider.slider-discrete .slider .slider-handle .slider-label span{font-size:12px}}.rkmd-slider.slider-discrete .slider .slider-handle.is-active{top:0;margin-left:-2px;width:4px;height:4px}.rkmd-slider.slider-discrete .slider .slider-handle.is-active .slider-label{top:-15px;left:-2px;border-radius:15px 15px 15px 0;-webkit-transform:rotate(-45deg) translate(23px,-25px);transform:rotate(-45deg) translate(23px,-25px)}.rkmd-slider.slider-discrete .slider .slider-handle.is-active .slider-label span{opacity:1}.rkmd-slider.slider-discrete.slider-turquoise .slider-label{background-color:#16a085}.rkmd-slider.slider-discrete.slider-emerald .slider-label{background-color:#27ae60}.peterriver{background:#3498db;border-bottom:#2980b9 3px solid}.rkmd-slider.slider-discrete.slider-peterriver .slider-label{background-color:#2980b9}.wetasphalt{background:#34495e;border-bottom:#2c3e50 3px solid}.rkmd-slider.slider-discrete.slider-wetasphalt .slider-label{background-color:#2c3e50}.sunflower{background:#f1c40f;border-bottom:#e6bb0f 3px solid}.rkmd-slider.slider-discrete.slider-sunflower .slider-label{background-color:#e6bb0f}.carrot{background:#e67e22;border-bottom:#d35400 3px solid}.rkmd-slider.slider-discrete.slider-carrot .slider-label{background-color:#d35400}.alizarin{background:#e74c3c;border-bottom:#c0392b 3px solid}.rkmd-slider.slider-discrete.slider-alizarin .slider-label{background-color:#c0392b}input{margin:0 auto 1.2rem auto;padding:2px 5px;width:100%;box-sizing:border-box;border:0;border-radius:4px;box-shadow:inset 0 0 6px rgba(0,0,0,0.3);background:rgba(255,255,255,0.8)}select{margin:0 auto 1.2rem auto;padding:2px 5px;width:100%;box-sizing:border-box;border:0;border-radius:4px;box-shadow:inset 0 0 6px rgba(0,0,0,0.3);background:rgba(255,255,255,0.8)}input[id^="num"]{max-width:6em;width:auto;text-align:right;font-weight:bold;font-size:115%}body div>ul.navigation{margin:0;margin-bottom:30px;padding:0;border-bottom:3px solid #666;overflow:hidden}ul.navigation li{list-style:none;float:left;margin-right:4px}ul.navigation li.controls{float:right}ul.navigation li a{font-weight:bold;display:inline-block;padding:6px 12px;color:#888;outline:0;text-decoration:none;background:#f3f3f3;background:-webkit-gradient(linear,0 0,0 bottom,from(#eee),to(#e4e4e4));background:-moz-linear-gradient(#eee,#e4e4e4);background:linear-gradient(#eee,#e4e4e4);-pie-background:linear-gradient(#eee,#e4e4e4)}ul.navigation li.active a{pointer-events:none;color:white;background:#666;background:-webkit-gradient(linear,0 0,0 bottom,from(#888),to(#666));background:-moz-linear-gradient(#888,#666);background:linear-gradient(#888,#666);-pie-background:linear-gradient(#888,#666)}div.tabscontent>div{padding:0 15px}#tabsnav:empty{display:none}.range-slider{margin:0}.range-slider{width:100%}.range-slider__range{-webkit-appearance:none;width:calc(100% - (45px));height:10px;border-radius:5px;outline:0;padding:0;margin:0}.range-slider__value{display:inline-block;position:relative;width:30px;color:#fff;line-height:20px;text-align:center;border-radius:3px;padding:5px 5px;margin-left:2px}.range-slider__value:after{position:absolute;top:8px;left:-7px;width:0;height:0;content:""}::-moz-range-track{border:0}input::-moz-focus-inner,input::-moz-focus-outer{border:0}svg{display:block;width:100%;height:100%}.y-axis path,.x-axis path{stroke:gray;stroke-width:1;fill:none}.series{stroke:steelblue;stroke-width:3;fill:none}.data-points circle{stroke:steelblue;stroke-width:2;fill:white}.data-points text{display:none}.data-points circle:hover{fill:steelblue;stroke-width:6}.data-points circle:hover+text{display:inline-block}text{text-anchor:end}.vert-switcher{transform:rotate(270deg);margin-top:15px;margin-bottom:25px}.vert-slider{width:150px;transform:rotate(270deg);display:inline-block;margin:50px -42px 70px -42px}.vert-slider span{transform:rotate(90deg)}button:disabled{color:#333;background-color:#999}select:disabled{color:#333;background-color:#999}input:disabled{color:#333;background-color:#999}.range-slider__range:disabled{background-color:#999}.range-slider__range:disabled::-webkit-slider-thumb{background-color:#aaa}.range-slider__range:disabled::-moz-range-thumb{background-color:#aaa}.switch.disabled .in::before{background:#bbb;border:1px solid #ddd}.switch.disabled .in::after{background:#bbb}.switch.checked.disabled{background:#b1d092}
)=====";

const uint8_t CSS_STYLE_GZIP[3597] PROGMEM = { 31,139,8,0,126,105,124,101,2,255,213,27,219,110,235,184,241,87,212,4,1,98,172,164,234,110,91,198,46,250,216,183,2,221,151,22,197,246,128,146,168,72,136,44,185,146,124,146,172,161,127,239,144,212,133,55,197,78,114,22,69,143,79,18,155,154,25,206,157,195,33,109,167,77,221,163,178,198,237,229,212,116,101,95,54,117,220,226,10,245,229,119,124,120,41,179,190,136,183,251,135,195,17,181,79,101,29,123,206,233,245,144,52,175,86,87,254,94,214,79,113,210,180,25,110,45,24,25,236,180,169,206,199,218,28,255,118,23,134,236,58,206,195,33,175,26,212,199,21,206,123,0,67,109,118,57,150,181,85,224,242,169,232,9,0,165,73,9,181,40,43,207,93,28,77,179,20,40,107,94,98,199,8,78,175,244,167,125,74,208,163,231,4,166,187,223,154,238,46,52,29,59,220,28,78,40,203,128,27,139,76,192,88,156,70,90,58,5,29,98,18,0,171,125,223,28,227,128,14,193,103,198,101,72,153,0,206,155,54,190,207,243,124,248,203,17,103,37,122,92,32,92,207,9,79,175,155,139,13,31,241,34,196,36,228,22,30,14,26,172,237,222,89,199,138,66,135,199,66,175,227,131,200,103,72,18,252,126,247,48,12,118,135,83,98,164,164,197,232,121,214,53,227,60,169,80,250,172,135,48,138,246,194,116,28,59,135,81,243,30,209,50,96,60,181,205,185,206,172,81,250,40,138,6,187,63,183,255,57,55,101,135,47,11,64,124,239,162,36,221,167,135,217,232,84,147,247,110,132,156,93,104,248,96,158,174,169,202,108,176,241,17,183,168,202,4,92,15,167,233,214,149,113,189,45,194,145,195,227,158,112,143,219,22,156,175,21,208,253,96,191,203,18,5,125,191,115,146,61,143,254,130,123,212,157,10,84,245,18,122,176,15,177,130,158,250,56,20,102,239,206,53,248,234,139,52,121,238,166,129,147,203,216,56,74,18,39,231,177,193,94,109,35,78,140,163,45,246,60,25,53,243,195,192,17,38,70,85,249,59,106,203,90,68,222,6,169,175,232,59,117,252,189,151,240,200,25,106,159,5,196,32,8,118,225,86,70,100,163,60,98,133,18,92,93,180,241,124,120,41,202,30,91,221,9,165,56,174,155,151,22,157,164,32,181,61,124,156,2,45,182,93,248,100,216,1,249,229,194,239,67,143,95,123,11,164,122,170,227,20,215,96,83,46,186,14,57,228,28,235,133,121,225,214,113,14,21,228,159,57,31,216,190,20,171,16,90,135,172,236,78,21,122,139,203,154,194,38,85,147,62,235,88,4,191,233,203,20,85,227,212,9,234,48,65,56,168,185,173,111,78,177,101,187,33,240,170,6,193,126,191,151,152,112,73,168,50,133,89,100,166,49,189,237,33,187,241,108,156,90,76,31,31,94,64,87,244,93,76,227,208,34,159,71,124,155,78,2,50,156,249,240,154,227,47,223,163,204,21,65,91,156,105,32,51,127,155,110,83,17,242,169,197,184,214,192,238,147,212,139,118,34,108,211,162,250,73,199,66,134,145,27,6,34,240,27,174,32,48,52,192,120,159,69,129,196,239,233,220,158,42,29,229,125,190,243,65,56,37,79,6,206,152,241,230,197,136,141,239,130,7,77,86,157,243,163,4,77,18,164,178,12,141,102,100,43,129,79,210,244,248,40,206,203,182,235,173,180,40,171,108,6,231,7,39,84,186,172,56,131,221,212,120,166,190,188,159,22,186,192,142,230,127,91,224,163,127,105,228,165,208,183,253,249,31,129,40,192,88,18,140,231,193,131,188,57,183,210,184,239,216,2,245,28,92,88,6,217,139,228,187,242,85,102,145,232,167,195,223,113,45,61,8,35,145,58,141,68,9,38,10,69,242,117,169,168,96,27,16,169,20,234,59,79,162,94,105,88,216,187,146,114,94,112,165,136,72,139,9,213,42,86,95,148,109,54,194,174,40,12,204,193,192,58,17,78,22,139,144,131,229,35,23,161,168,230,154,60,239,112,111,37,111,150,224,9,154,209,78,112,157,157,228,25,11,198,226,35,166,118,84,164,227,110,37,86,23,12,206,147,204,149,113,145,150,23,9,4,56,143,51,245,195,34,186,31,216,43,34,113,142,105,234,135,69,74,129,191,38,212,226,191,166,118,84,164,19,122,34,50,231,98,230,202,184,72,32,114,214,68,226,163,193,92,25,151,104,237,215,132,226,162,198,212,15,139,148,182,162,227,245,90,161,250,21,145,118,209,170,72,213,138,126,164,216,228,169,237,195,53,161,228,16,52,223,121,118,171,31,41,241,106,190,247,240,86,245,75,209,109,174,63,82,189,107,40,250,99,117,161,165,11,84,75,56,142,60,59,124,24,146,38,123,27,65,161,178,94,158,186,54,169,44,132,194,70,40,123,96,197,99,159,115,116,44,171,183,248,238,111,39,92,27,191,162,186,187,51,59,248,13,158,218,150,249,84,52,121,164,134,84,151,222,52,119,114,119,40,92,179,240,204,194,55,139,192,44,66,179,136,38,214,73,137,227,72,117,140,29,182,192,23,207,137,239,56,64,130,19,44,104,101,206,109,239,80,225,30,138,56,90,229,144,122,15,42,39,128,26,10,143,195,243,237,72,131,25,174,161,250,60,170,138,232,175,225,5,28,158,103,107,152,245,53,83,58,59,138,27,94,120,11,237,84,92,29,106,72,81,35,1,53,84,81,35,25,213,89,173,92,4,117,135,138,34,3,219,83,53,68,149,43,74,239,43,50,121,43,172,14,195,73,112,138,1,141,91,198,123,23,35,156,37,3,138,139,134,236,185,198,81,39,71,78,138,135,228,12,62,83,95,180,197,247,84,249,187,234,246,221,23,246,210,250,242,122,164,29,227,26,37,21,206,98,148,146,170,252,162,221,143,30,122,168,84,187,188,105,143,49,125,7,21,60,254,199,99,0,154,52,230,207,255,164,159,135,251,35,20,132,127,197,8,184,209,178,61,220,67,209,248,107,143,250,115,183,244,59,202,186,128,96,235,185,232,181,183,16,189,35,139,166,61,170,65,218,16,16,15,224,115,132,237,115,35,172,220,164,67,131,125,182,242,115,85,49,23,224,43,153,149,70,202,217,34,173,0,14,101,233,12,188,143,118,34,40,116,230,11,235,184,208,247,243,19,194,229,133,111,197,244,108,103,118,81,246,106,67,209,242,238,194,188,93,18,159,250,232,100,120,198,157,51,125,36,72,238,180,205,52,238,177,75,94,195,82,175,199,40,135,89,76,187,109,94,166,183,103,43,205,47,4,0,24,136,239,238,230,13,95,79,220,227,144,86,24,181,32,109,95,48,42,109,83,233,246,45,89,198,59,91,121,68,79,56,38,182,71,100,99,4,190,9,180,31,11,112,151,71,199,116,30,232,127,219,221,152,203,8,81,47,29,219,72,30,29,78,106,103,13,169,178,134,69,195,112,12,34,227,244,163,80,9,55,166,195,193,40,207,183,28,128,167,3,240,56,0,255,26,64,112,13,32,18,0,38,14,166,94,208,126,54,112,236,195,30,30,157,251,230,176,214,15,220,19,159,30,173,96,156,171,203,210,200,123,152,179,130,195,197,108,219,64,188,225,199,32,204,240,211,102,193,172,202,139,168,100,66,192,32,236,58,170,174,45,34,130,117,139,174,53,50,106,243,215,200,53,177,44,201,124,164,237,19,23,101,150,225,250,48,245,4,31,120,49,129,223,184,238,11,182,53,124,244,54,23,69,192,189,35,10,40,227,248,26,28,235,26,82,160,65,114,119,42,18,154,204,224,81,51,168,237,14,101,102,102,143,81,88,130,181,208,27,87,3,147,27,200,155,20,82,166,18,115,58,151,227,232,92,212,48,26,99,21,42,179,217,46,204,32,36,251,66,136,87,153,225,226,227,159,125,131,171,130,212,102,18,29,201,112,218,180,136,202,89,67,249,198,6,231,158,241,170,179,4,27,166,13,166,33,219,13,187,133,99,242,38,47,219,227,143,202,46,161,38,189,128,138,54,63,60,155,120,255,243,108,178,131,72,99,197,50,88,153,47,141,166,126,94,108,177,162,120,118,78,148,192,2,113,238,89,47,142,96,141,91,119,62,191,76,6,145,61,114,30,95,113,204,123,140,241,76,5,220,163,127,180,193,221,104,173,177,49,208,140,189,90,118,108,183,219,85,116,8,208,247,241,186,151,178,79,11,109,1,98,252,169,60,158,154,182,71,117,175,41,140,18,76,94,210,226,19,136,167,33,147,187,56,212,8,244,68,4,140,64,94,182,191,225,235,174,244,220,118,240,254,212,148,52,100,212,104,27,43,68,66,127,74,30,209,114,82,242,110,81,179,186,56,68,164,28,132,173,69,242,92,246,22,23,102,178,172,134,237,117,6,70,29,182,128,112,115,238,15,214,177,249,253,99,8,205,7,233,119,31,130,255,0,236,100,112,59,45,112,250,172,237,213,110,163,204,115,179,9,208,40,235,211,185,255,87,255,118,194,63,223,81,36,176,239,221,111,179,195,208,108,38,153,111,94,101,65,191,172,29,68,206,111,244,177,180,131,39,83,193,72,26,158,101,125,89,7,92,136,105,204,70,30,26,176,123,122,223,86,43,80,205,45,148,186,235,64,215,0,100,253,27,89,249,253,194,250,11,59,34,254,168,116,80,67,156,96,88,3,197,83,45,113,139,18,83,217,228,180,14,186,50,9,160,225,192,31,135,188,12,146,20,55,2,226,168,189,25,137,17,49,41,203,132,0,125,195,202,103,147,245,201,59,24,126,116,40,229,13,63,68,19,238,56,209,70,59,201,167,24,108,62,135,6,38,250,12,158,138,51,9,175,199,100,167,147,220,182,129,26,70,200,132,158,124,46,236,44,167,194,66,14,156,183,17,83,212,236,86,99,197,10,167,24,176,246,115,216,144,60,40,248,13,221,165,136,110,67,153,191,209,115,152,160,204,210,63,214,115,152,143,104,156,7,92,234,3,158,115,133,193,230,115,104,171,158,243,62,222,123,158,163,195,148,118,17,180,251,160,120,128,55,175,108,49,221,71,16,231,33,111,52,110,49,157,248,19,31,104,159,143,153,213,129,63,114,13,133,177,1,162,172,128,220,154,74,214,81,161,189,247,247,6,68,104,248,222,158,64,91,88,17,232,185,28,44,7,242,190,100,149,211,101,1,39,111,27,210,127,234,223,200,225,200,149,25,126,178,111,148,140,235,62,76,149,194,86,105,251,184,190,246,66,1,171,105,166,94,216,184,156,197,57,216,120,115,185,153,193,113,214,64,153,212,25,110,150,210,24,255,90,121,89,85,107,34,75,170,117,14,252,238,246,220,145,246,30,174,112,218,179,53,250,119,88,129,50,252,26,187,31,103,162,64,117,86,225,139,180,202,235,115,20,117,94,118,224,53,153,34,228,108,30,74,69,155,181,83,212,68,170,107,110,25,69,85,53,23,49,235,82,121,95,51,154,44,42,203,182,54,189,41,115,141,70,60,85,219,179,7,104,28,203,33,47,174,160,254,56,85,193,37,204,207,163,139,198,204,112,142,206,85,127,165,212,255,193,220,207,31,232,65,252,151,101,17,201,137,149,233,31,47,140,93,118,22,219,98,125,89,144,133,20,231,128,183,70,209,15,19,227,7,91,103,141,176,100,39,125,10,160,93,32,2,48,124,149,141,121,27,188,212,100,132,46,239,248,203,140,93,138,42,252,232,110,214,188,100,34,13,115,166,45,238,103,201,228,32,83,87,39,125,22,190,145,158,164,192,149,34,209,221,218,115,157,24,204,254,67,78,115,38,255,241,149,141,47,145,218,106,96,211,12,37,7,40,159,86,43,135,245,39,170,181,20,106,139,14,165,38,222,117,8,205,222,78,88,4,86,198,175,175,0,159,83,51,139,48,79,167,178,81,4,210,242,186,38,166,6,102,248,186,15,24,221,9,173,109,152,183,202,66,236,44,61,205,249,106,219,82,3,170,157,75,85,94,161,63,190,214,55,95,170,186,153,130,178,110,211,13,136,102,180,83,7,229,129,63,200,206,76,149,156,78,60,121,241,191,145,172,148,197,29,49,83,123,115,68,114,189,172,64,74,223,31,158,73,114,89,150,7,150,221,162,167,22,192,33,233,206,206,191,156,117,91,143,206,186,28,95,62,122,80,60,155,150,71,238,24,31,62,6,254,35,133,100,246,154,92,237,253,124,58,125,158,239,10,75,250,82,171,4,118,83,248,38,162,227,21,226,171,36,217,5,226,47,95,27,190,129,163,101,134,235,76,81,250,95,190,140,124,3,83,203,12,215,153,162,244,191,122,197,249,6,158,230,9,174,178,196,200,127,233,222,244,13,252,48,234,87,153,97,180,191,120,19,251,6,118,38,250,87,25,98,212,7,90,150,205,55,140,232,201,172,193,14,223,199,83,218,241,196,149,28,250,112,149,237,234,69,129,195,252,53,128,47,156,50,112,186,97,95,201,8,67,115,250,113,236,221,102,96,75,203,255,29,219,172,6,46,179,127,255,124,87,159,143,119,191,113,87,47,34,124,28,153,164,252,115,11,59,61,22,17,174,84,145,83,76,190,18,112,199,139,98,164,49,253,203,185,178,107,244,189,124,162,7,151,203,213,49,241,122,133,207,125,139,101,145,121,122,184,244,72,201,237,24,169,77,52,8,19,144,99,246,170,236,128,147,254,173,194,108,241,95,238,128,136,39,59,100,197,148,145,167,51,176,78,184,82,34,67,25,232,162,200,255,238,173,33,98,29,218,213,24,157,125,183,219,29,154,115,79,32,201,221,1,221,233,174,144,174,124,242,186,165,135,74,122,133,206,212,62,204,219,230,248,72,78,6,55,102,223,192,155,128,188,164,54,169,174,137,75,80,204,9,252,189,78,165,8,104,157,74,144,251,70,104,85,243,227,2,141,46,99,81,102,145,171,153,125,55,158,14,81,189,209,175,30,8,154,33,14,241,57,181,128,9,152,90,128,196,45,58,1,120,147,194,190,171,144,5,234,170,54,102,208,1,194,196,238,161,238,30,219,184,191,144,243,156,57,24,104,113,53,220,147,231,160,175,24,31,79,253,155,176,239,133,52,76,182,175,83,215,118,138,48,105,120,73,56,226,131,111,223,216,215,34,38,205,161,211,9,216,68,117,58,6,15,195,131,237,71,74,59,237,134,101,64,133,14,101,216,70,56,161,147,54,115,164,43,59,123,247,18,214,122,214,190,125,251,142,200,119,67,244,1,180,210,155,245,197,47,176,9,7,240,244,251,111,234,78,68,189,180,55,113,22,142,73,89,184,49,78,203,105,149,205,241,104,228,202,249,162,181,157,115,252,220,82,117,184,54,253,16,51,47,99,19,64,157,155,62,207,223,87,99,89,121,4,160,199,253,176,223,175,113,107,170,227,160,100,188,124,209,109,232,190,63,73,125,94,181,145,205,60,224,205,66,175,101,103,156,80,95,152,246,235,242,225,210,65,242,123,198,49,120,233,219,129,189,159,174,226,29,72,135,103,116,56,210,208,199,221,4,220,245,24,87,228,235,61,34,134,207,99,100,168,71,22,13,236,206,72,203,54,173,240,21,108,143,97,211,136,23,209,137,101,165,0,80,169,143,247,59,41,141,149,25,162,117,188,159,132,57,132,11,149,244,9,115,174,58,45,192,253,112,13,133,16,249,30,150,197,206,207,200,5,67,121,67,227,109,233,13,166,3,119,201,144,239,191,141,235,156,71,191,173,195,72,9,65,27,82,135,94,35,170,141,155,49,212,8,166,97,5,164,234,216,206,111,133,57,216,22,104,237,114,215,120,107,117,234,139,93,184,219,76,250,107,174,227,30,251,118,4,230,212,183,195,235,146,215,130,254,9,156,120,94,52,166,13,94,113,62,38,26,74,8,161,171,148,184,152,126,143,202,120,126,63,33,210,35,87,221,89,125,146,36,154,131,226,44,203,86,72,168,199,182,64,65,190,46,96,107,180,5,128,110,230,236,189,225,191,54,182,22,18,32,61,0,0 };
