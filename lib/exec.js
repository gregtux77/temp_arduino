const Promise = require('bluebird');
//const sendcode = require('sendcode2.js')(code1, code2);
const retry = 4;
 

// wait time between signals in ms
var shared = require('./shared.js');

 function sendcode2(code1,code2) {

  // geting the first arduino finded on usb
  var ports = shared.getPorts();
  var port = ports[0];
  // write the code to the arduino
  port.write(code1,code2);

};

const waitTimeBetweenSignals = 200;






module.exports = function(params){

	// if module serial is not present, we cannot contact the arduino
	if(!gladys.modules.serial || typeof gladys.modules.serial.sendCode !== 'function') {
		sails.log.error(`You need to install the serial module in Gladys.`);
		return Promise.reject(new Error('DEPENDENCY_MISSING'));
	}

	var code = parseInt(params.deviceType.identifier) + parseInt(params.state.value);
	var prise = 1
	var etat = 1
	
	// radio is not secure transmision, we send the signals many time to be sure the signal is sent
	for (var i = 0; i <= retry; i++) {
		setTimeout(function(){ 
			//gladys.modules.serial.sendCode(`{"function_name":"SendRadioCode","code":"${code}"}%`);
			sendCode2(`{"function_name":"SendRadioCode","prise":"${prise}","etat":"${etat}"}%');
		}, i * waitTimeBetweenSignals);
	}
	
	return Promise.resolve();
};
