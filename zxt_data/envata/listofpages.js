
var total_pages = 1307;
var _url_start = 'https://themeforest.net/authors/top?level=all&page=';
var _url_ends =  '&site=all';
var download_folder = __dirname + '/downloads/lists/';
//console.log(download_folder)
for (i = 1; i <= total_pages;i++) {
  var _download_url = _url_start + i + _url_ends;
  var _file = download_folder + i + '.html'
  var _cmd = 'wget -O "' + _file + '" "' + _download_url + '"';
  console.log(_cmd);
}

