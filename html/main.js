const ecgSvcUUID16 = 0x1857;
const ecgVoltageCharUUID16 = 0x2BDD;
const measurementIntervalCharUUID16 = 0x2A21;
const objectSizeCharUUID16 = 0x2BDE;

function parseEcg(dataView) {
  if (dataView.byteLength <= 4) {
    console.error(`Invalid ECG frame: Length of ${dataView.byteLength}`, dataView);
    return null;
  }
  const time = dataView.getUint32(0, true);
  const voltages = [];
  const size = (dataView.byteLength - 4) / 2;
  const n = size * 2 + 4;
  for (let i = 4; i < n; i += 2) {
    const voltage = dataView.getInt16(i, true);
    voltages.push(voltage);
  }
  return {
    t: time,
    v: voltages,
  };
}

function reduceEcg(packet, delta) {
  const time = packet.t;
  const voltages = packet.v;
  const n = voltages.length;
  const result = [];
  for (let i = 0; i < n; i++) {
    const v = voltages[i];
    const t = time - (n - i - 1) * delta;
    const value = { t, v };
    result.push(value);
  }
  return result;
}

function parseInterval(dataView) {
  return dataView.getUint16(0, true);
}

function parseSize(dataView) {
  return dataView.getUint16(0, true);
}

async function getSamplingInterval(gattService) {
  const characteristic = await gattService.getCharacteristic(measurementIntervalCharUUID16);
  const value = await characteristic.readValue();
  return parseInterval(value);
}

async function getPacketSize(gattService) {
  const characteristic = await gattService.getCharacteristic(objectSizeCharUUID16);
  const value = await characteristic.readValue();
  return parseSize(value);
}

async function subscribeToEcg(gattService, callback) {
  const characteristic = await gattService.getCharacteristic(ecgVoltageCharUUID16);
  characteristic.addEventListener('characteristicvaluechanged', callback);
  await characteristic.startNotifications();
  return {
    characteristic,
    callback,
  };
}

async function unsubscribeFromEcg(listener) {
  const characteristic = listener.characteristic;
  const callback = listener.characteristic;
  await characteristic.stopNotifications();
  characteristic.removeEventListener('charachteristicvaluechagned', callback);
}

async function connectToDevice() {
  const device = await navigator.bluetooth.requestDevice({
    filters: [{
      services: [ecgSvcUUID16]
    }]
  });
  return await device.gatt.connect();
}

function onEcgData(evt) {
  const ecg = parseEcg(evt.target.value);
  const ecgReduced = reduceEcg(ecg, interval);
  console.log('ecg', ecg, evt.target.value, ecgReduced);
  for (const value of ecgReduced) {
    result += `${value.t}\t${value.v}\n`;
  }
}

var server;
var service;
var interval;
var ecgListener;
var size;

var result = 't\tv\n';

async function connect() {
  server = await connectToDevice();
  service = await server.getPrimaryService(ecgSvcUUID16);
  size = await getPacketSize(service);
  interval = await getSamplingInterval(service);
  ecgListener = await subscribeToEcg(service, onEcgData);
}

async function disconnect() {
  await unsubscribeFromEcg(ecgListener);
  await service.disconnect();
}

function copy() {
  navigator.clipboard.writeText(result);
}
