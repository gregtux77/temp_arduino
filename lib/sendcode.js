

module.exports = function(prise,etat) {

  // geting the first arduino finded on usb
  var ports = shared.getPorts();
  var port = ports[0];
  // write the code to the arduino
  port.write(prise,etat);
    
};
