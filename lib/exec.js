const Promise = require('bluebird');

const retry = 4;

// wait time between signals in ms
const waitTimeBetweenSignals = 200;

module.exports = function(params){

 
		 module.exports =  function temperature() {
                // do something here
                gladys.deviceType.getById({id: 18})
                        .then((deviceType) => {
                        var temp = deviceType.lastValue;
         gladys.modules.telegram.notify({ text: 'la temperature de la maison est de ' + temp +  ' degres' },{ id: 1 })

        console.log(`la temperature de la maison est de  ${deviceType.lastValue} degres`);
	            
   });
}
	}
	
	return Promise.resolve();
};
