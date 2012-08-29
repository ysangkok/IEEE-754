"use strict";

var utils = require("utils");

var casper = require('casper').create();

casper.on('page.error', function(msg, trace) {
    console.error(msg);
    trace.forEach(function _forEach(item) {
        console.log('  ', item.file, ':', item.line);
    });
});

function testNumber(input, output) {
	this.input = input;
	this.output = output;
	if (input.click === undefined) input.click=["#force-auto"];
	this.results = [];
}

var testCases = [
  new testNumber({entered:'1.0625'}, [{key:"#binary_value", value: '1.0001'}]),
  new testNumber({entered:'1.25'}, [{key:"#binary_value", value: '1.01'}])
];

var queuePageTests = function(pageNumber) {
	testCases.map(function(i) {
		casper.then(function() {
			console.log("Page number: " + pageNumber);
		});
		var input = i.input;
		var output = i.output;
		var oldvalue;

		//utils.dump(input);
		//utils.dump(output);

		casper.then(function() {
			console.log(input.entered);
		});

		casper.thenEvaluate(
			function(term) {
		        document.getElementById('value-entered').setAttribute("value",term);
			},
			{term: input.entered}
		);

		casper.then(function() {
		        oldvalue = this.fetchText(output[0].key);
			console.log("Old value in output field: " + oldvalue);
			var outer = this;
		        input.click.map(function(j) {
				outer.click(j);
			});
		});

		casper.then( function() {
			console.log("Waiting for busy-spin animation to disappear...");
		});
		casper.waitWhileVisible(".idle");
		casper.then( function() {
			console.log("Waiting for new result to appear in output field...");
		});
		casper.waitFor(
			function check() {
				var value = this.fetchText(output[0].key);
				console.log("Current value: " + value);
				return value !== oldvalue;
			},
			function then() {},
			function timeout() {
				this.test.assertTrue(false,"timeout");
			}
		);

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

//casper.start("http://ysangkok.github.com/IEEE-754/index.xhtml");
casper.start("http://localhost:8800/IEEE-754/index.xhtml");
queuePageTests(0);
//casper.thenOpen('http://localhost:8800/I2/');

casper.run(function() {
	this.test.renderResults(true);
});

