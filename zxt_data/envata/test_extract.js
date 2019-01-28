const fs = require('fs-extra');
const xpath = require('xpath');
const parse5 = require('parse5');
const xmlser = require('xmlserializer');
const dom = require('xmldom').DOMParser;

const html =  fs.readFileSync('./downloads/lists/1.html');
const document = parse5.parse(html.toString());
const xhtml = xmlser.serializeToString(document);
const doc = new dom().parseFromString(xhtml);
const select = xpath.useNamespaces({"x": "http://www.w3.org/1999/xhtml"});
const nodes = select('//x:ul[@class="user-list"]>li[*]', doc);
//console.log(nodes);
for (_i in nodes) {
  //console.log(nodes[_i].localName + " : " + nodes[0].firstChild.data);
  console.log(nodes[_i])
  break;
}



