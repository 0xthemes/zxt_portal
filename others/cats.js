
const fs = require('fs-extra');
const he = require('he');
const _ = require('underscore');
var Q = require('q');
var jp = require('jsonpath');
var colors = require('colors');
var endOfLine = require('os').EOL;

const SKIP_DUPE = true;




const argumentsDefs = [{
    name: 'style',
    alias: 's',
    type: Number
  }
];
const commandLineArgs = require('command-line-args');
const arguments = commandLineArgs(argumentsDefs);



const cats = JSON.parse(fs.readFileSync('./data/cats.json', 'utf8')).categories;
var id_inc = 1;
var end_content = "";
for(_i in cats) {
  cats[_i].id = id_inc;
  cats[_i].parent_id = 0;
  cats[_i].depth = (cats[_i].path.split("/").length) - 1;
  varad = cats[_i];
  id_inc++;
  var intent = "";
  if (cats[_i].depth) {
    intent = "-".repeat(cats[_i].depth);
  }
  cats[_i].taxo = intent + cats[_i].name;
  end_content += cats[_i].taxo + endOfLine;
}

console.log((end_content));




return 0;

var count_total_ico = 0;
var total_tbd = 0;
var all_icos = [];
//console.log(colors.red(icos));
for (nid in pages) {
  var page = pages[nid];

  var data = JSON.parse(page.field_json);
  if (_.has(data, 'results') && _.has(data.results, 0) && _.has(data.results[0], 'hits') && data.results[0].hits.length) {
    for (_icoindex in data.results[0].hits) {
      var ico = data.results[0].hits[_icoindex];

      if (ico.tbd) {
        total_tbd++;
      }
      count_total_ico++;
      all_icos.push(ico);
    }
  }
  //break;
}
console.log("total ICOs", colors.red(all_icos), total_tbd);
var json_String = JSON.stringify(all_icos);
fs.writeJsonSync('./all_icos.json', all_icos);


return 0;
//var final = jp.query(node_style, '$..node..nid');
var style_id = arguments.style ? arguments.style : null;
if (!style_id) {
  return 0;
}
var style = jp.query(node_style, '$..*[?(@.nid==' + style_id + ')]');
if(_.has(style, 0)) {

  var dir_component = dir_components + '/' + (style[0].field_style_name ? style[0].field_style_name : style[0].nid);
  fs.ensureDirSync(dir_component);
  var dir_screenshoots = dir_component + '/screenshot';
  fs.ensureDirSync(dir_screenshoots);
  //Download the mobile screen
  if (style[0].field_mobile_screen_url && !SKIP_DUPE) {
    var http_req = requestthen('GET', style[0].field_mobile_screen_url);
    var filename = dir_screenshoots + '/mobile.jpg'
    fs.writeFileSync(filename, http_req.getBody());
  }
  if (style[0].field_screenshot && !SKIP_DUPE) {
    var http_req = requestthen('GET', style[0].field_screenshot);
    var filename = dir_screenshoots + '/desktop.jpg'
    fs.writeFileSync(filename, http_req.getBody());
  }

  var dir_sources = dir_component + '/sources';
  fs.ensureDirSync(dir_sources);

  //All FC manipulations goes here
  const fc_field_theme_files= JSON.parse(fs.readFileSync('./q2data/field_collection_item_field_theme_files.json', 'utf8')).nodes;

  //Download all the source files
  var all_sourcefiles = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="110" || @.field_file_type=="109"))].field_files');
  if (all_sourcefiles && !SKIP_DUPE) {
    for (_index in all_sourcefiles) {
      if (all_sourcefiles[_index]) {
        _all_sourcefiles = all_sourcefiles[_index].split('@@@@');
        for (__index in _all_sourcefiles) {
          if (_all_sourcefiles[__index]) {
            var _all_sourcefiles_filename = dir_sources + '/' + _all_sourcefiles[__index].substring(_all_sourcefiles[__index].lastIndexOf('/') + 1);
            var http_req = requestthen('GET', _all_sourcefiles[__index]);
            fs.writeFileSync(_all_sourcefiles_filename, http_req.getBody());
          }
        }
      }
    }
  }

  //*************************************************************** */
  //********************v  Drupal 8 Stuffs  *********************** */
  //*************************************************************** */
  var dir_drupal8 = dir_component + '/drupal8';
  fs.ensureDirSync(dir_drupal8);
  var dir_drupal8_folders = [
    'assets',
    'assets/images',
    'assets/html',
    'assets/fonts',
    'css',
    'css/images',
    'js',
    'templates',
    'snippets',

  ];
  //create all the drupal 8 folders
  for (_index in dir_drupal8_folders) {
    var _dir_drupal8_folders = dir_drupal8 + '/' + dir_drupal8_folders[_index];
    fs.ensureDirSync(_dir_drupal8_folders);
  }
  //Content images 194 and CSS image 193 and logo images : 187
  var all_content_images = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="194" || @.field_file_type=="193" || @.field_file_type=="187"))]');
  if (all_content_images && all_content_images.length && !SKIP_DUPE) {
    for (_index in all_content_images) {
      var http_req = requestthen('GET', all_content_images[_index].field_files);
      var _all_content_images_file = dir_drupal8 + '/' + all_content_images[_index].field_file_path;
      fs.writeFileSync(_all_content_images_file, http_req.getBody());
    }
  }
  //Css Files
  var css_terms = [];
  css_terms[183] = 'style';
  css_terms[184] = 'responsive';
  css_terms[185] = 'colors';
  var all_css = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="183" || @.field_file_type=="184" || @.field_file_type=="185"))]');
  if (all_css && all_css.length) {
    for (_index in all_css) {
      var _cur_filetype_id = all_css[_index].field_file_type;
      var _cur_css_code = he.decode(all_css[_index].field_code);
      var _cur_css_filename = dir_drupal8 + '/css/'  + css_terms[_cur_filetype_id] + '.css';
      fs.writeFileSync(_cur_css_filename, _cur_css_code);
    }

  }
  //JS file
  var all_js = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="192"))].field_code');
  if (all_js && all_js.length) {
    for (_index in all_js) {
      var _cur_js_code = he.decode(all_js[_index]);
      var _cur_js_filename = dir_drupal8 + '/js/script.js';
      fs.writeFileSync(_cur_js_filename, _cur_js_code);
    }
  }
  //.Block snippets
  //theme page_hooks
  //file name to be _page_hooks.theme : 188
  var _fc_items = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="188"))]');
  if (_fc_items && _fc_items.length) {
    for (_index in _fc_items) {
      var _cur_code = he.decode(_fc_items[_index].field_code);
      var _cur_filename = _fc_items[_index].field_short_description;
      _cur_filename = dir_drupal8 + '/assets/html/' + _cur_filename + '.txt';
      fs.writeFileSync(_cur_filename, _cur_code);
      console.log(_cur_filename, colors.green(_cur_code));
    }
  }


  //.theme hooks
  //theme page_hooks
  //file name to be _page_hooks.theme : 190
  var _fc_items = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="190"))].field_code');
  if (_fc_items && _fc_items.length) {
    for (_index in _fc_items) {
      var _cur_code = he.decode(_fc_items[_index]);
      var _cur_filename = dir_drupal8 + '/snippets/_page_hooks.theme';
      fs.writeFileSync(_cur_filename, _cur_code);
    }
  }


  //Twigs
  //Frontpage twig snippets
  //file name to be _frontpage_regions.html.twig : 189
  var _fc_items = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="189"))].field_code');
  if (_fc_items && _fc_items.length) {
    for (_index in _fc_items) {
      var _cur_code = he.decode(_fc_items[_index]);
      var _cur_filename = dir_drupal8 + '/snippets/_frontpage_regions.html.twig';
      fs.writeFileSync(_cur_filename, _cur_code);
    }
  }

  //THEME/config/install/THEME.settings.yml
  //Frontpage twig snippets
  //file name to be _THEME.settings.yml : 200
  var _fc_items = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="200"))].field_code');
  if (_fc_items && _fc_items.length) {
    for (_index in _fc_items) {
      var _cur_code = he.decode(_fc_items[_index]);
      var _cur_filename = dir_drupal8 + '/snippets/_THEME.settings.yml';
      fs.writeFileSync(_cur_filename, _cur_code);
    }
  }


  //Regions block config for drupal 8
  //Json Settings : 186 for block regions, 201 for default block visibity 173 for color codes
  //file name to be _frontpage_regions.html.twig : 189
  var _fc_items = jp.query(fc_field_theme_files, '$..*[?(@.host_entity_id==' + style_id + ' && @.host_entity_type=="node" && (@.field_file_type=="189"))].field_code');
  if (_fc_items && _fc_items.length) {
    for (_index in _fc_items) {
      var _cur_code = he.decode(_fc_items[_index]);
      var _cur_filename = dir_drupal8 + '/snippets/_frontpage_regions.html.twig';
      fs.writeFileSync(_cur_filename, _cur_code);
    }
  }

  var config_json = {};

  //Create the directory for this component
  //console.log(colors.green(style[0]));
}
else {
  console.log(colors.red('no such styles'));
}




