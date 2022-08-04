const ecgSvcUUID16 = 0x1857;
const ecgVoltageCharUUID16 = 0x2BDD;
const measurementIntervalCharUUID16 = 0x2A21;
const objectSizeCharUUID16 = 0x2BDE;

function parseSize()

async function connect() {  
  const device = await navigator.bluetooth.requestDevice({
    filters: [{
      services: [ecgSvcUUID16]
    }]
  });
  console.log('device', device);
  const server = await device.gatt.connect();
  console.log('server', server);
  const service = await server.getPrimaryService(ecgSvcUUID16);
  console.log('service', service);
  const intervalCharacteristic = await service.getCharacteristic(measurementIntervalCharUUID16);
  console.log('intervalChar', intervalCharacteristic);
  const ecgCharacteristic = await service.getCharacteristic(ecgVoltageCharUUID16);
  console.log('ecgChar', ecgCharacteristic);
  const sizeCharacteristic = await service.getCharacteristic(objectSizeCharUUID16);
  console.log('sizeChar', ecgCharacteristic);
  const size = await sizeCharacteristic.readValue();
  console.log('size', size);
  const interval = await intervalCharacteristic.readValue();
  console.log('interval', interval);
  await ecgCharacteristic.startNotifications();
  ecgCharacteristic.addEventListener('characteristicvaluechanged', (evt) => {
    console.log('ecg', evt.target.value);
  });
}
