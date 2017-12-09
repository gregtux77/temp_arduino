const Promise = require('bluebird');
var exec = require('child_process').exec;

 module.exports =  function temperature() {
                // do something here
                gladys.deviceType.getById({id: 18})
                        .then((deviceType) => {
                        var temp = deviceType.lastValue;
         gladys.modules.telegram.notify({ text: 'la temperature de la maison est de ' + temp +  ' degres' },{ id: 1 })

        console.log(`la temperature de la maison est de  ${deviceType.lastValue} degres`);
	            
   });
}
//

