"use strict";

var utils = require("utils");

var casper = require('casper').create();


function testNumber(input, output) {
	this.input = input;
	this.output = output;
	if (input.click === undefined) input.click=["#force-auto"];
	this.results = [];
}

var arr = [
  new testNumber({entered:'1.0625'}, [{key:"#binary_value", value: '1.0001'}]),
  new testNumber({entered:'1.25'}, [{key:"#binary_value", value: '1.01'}])
];

var queuePageTests = function(pagenr) {
arr.map(function(i) {
casper.then(function() {
console.log("pagenr: " + pagenr);
});
var input = i.input;
var output = i.output;
var oldvalue;

//utils.dump(input);
//utils.dump(output);

casper.then(function() { console.log(input.entered); });

casper.thenEvaluate(function(term) {
        document.getElementById('value-entered').setAttribute("value",term);
},{term: input.entered});

casper.then(function(term) {
        oldvalue = this.fetchText(output[0].key);
	console.log("oldvalue: " + oldvalue);
	var outer = this;
        input.click.map(function(j) { outer.click(j); });
});

casper.then(function(){console.log("wait for idle");});
casper.waitWhileVisible(".idle");
casper.then(function(){console.log("waiting for new result");});
casper.waitFor(function check() {
    var value = this.fetchText(output[0].key);
    console.log(value);
    return value !== oldvalue;
}, function then() {}, function timeout(){this.test.assertTrue(false,"timeout");});

casper.then(function() {
    var outer = this;
    output.map( function (v) {
      outer.test.assertEquals(outer.fetchText(v.key), v.value);
    });
});

/*
casper.then(function() {
    this.captureSelector(i + '.ref.png', 'html');
});
*/
casper.then(function() {
	i.results.push(this.getPageContent());
});
});
};


casper.start("http://babbage.cs.qc.cuny.edu/IEEE-754/");
//casper.start('http://localhost:8800/IEEE-754/');
queuePageTests(0);
//casper.thenOpen('http://localhost:8800/I2/');

casper.run(function() {
	this.test.renderResults(true);
});

