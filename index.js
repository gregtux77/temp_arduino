var install = require('./lib/install.js');
var uninstall = require('./lib/uninstall.js');

var command = require('./lib/commande.js');
 var exec = require('./lib/exec.js');
    
  
      
   
module.exports = function(sails) {
  var sendCode = require('./lib/sendCode.js');

	 	
	return {
		
		command:command,
		install: install,
		uninstall: uninstall,
		sendcode: sendCode,
		  exec: exec
	};
};


