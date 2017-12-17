const Promise = require('bluebird');
//const sendcode = require('sendcode2.js')(code1, code2);
const retry = 4;
 

// wait time between signals in ms
var shared = require('./shared.js');



const waitTimeBetweenSignals = 200;






module.exports = function(params){
	 
	var prise = 0
	var etat = 1
	
	var sendCode2= (`{"function_name":"SendRadioCode","prise":"${prise}","etat":"${etat}"}%');
			var ports = shared.getPorts();
			var port = ports[0];
				//write the code to the arduino
			port.write(sendcode2);

	}
	
	return Promise.resolve();
};
