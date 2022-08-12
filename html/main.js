const ecgSvcUUID16 = 0x1857;
const ecgVoltageCharUUID16 = 0x2BDD;
const measurementIntervalCharUUID16 = 0x2A21;
const objectSizeCharUUID16 = 0x2BDE;
const movSvcUUID16 = 0x1858;
const movAccCharUUID16 = 0x2BDF;
const movGyrCharUUID16 = 0x2BE0;
const movMagCharUUID16 = 0x2BE1;
const activitySvcUUID16 = 0x1859;
const movCharUUID16 = 0x2BE2;
const ecgMeasurementIntervalCharUUID16 = 0x2BE3;
const movMeasurementIntervalCharUUID16 = 0x2BE4;

const accScale = 100.0;
const gyrScale = 10.0;
const magScale = 100.0;


function parseEcg(dataView) {
  if (dataView.byteLength <= 4) {
    console.error(`Invalid ECG frame: Length of ${dataView.byteLength}`, dataView);
    return null;
  }
  const time = dataView.getUint32(0, true);
  const voltages = [];
  const n = dataView.byteLength;
  for (let i = 4; i < n; i += 2) {
    const voltage = dataView.getInt16(i, true);
    voltages.push(voltage);
  }
  return {
    t: time,
    v: voltages,
  };
}

function parseMovement(dataView) {
  if (dataView.byteLength <= 4) {
    console.error(`Invalid Movement frame: Length of ${dataView.byteLength}`, dataView);
    return null;
  }
  const time = dataView.getUint32(0, true);
  const movement = [];
  const n = dataView.byteLength;
  for (let i = 4; i < n; i += (2 * 3 * 3)) {
    const mov = {
      acc: {
        x: dataView.getInt16(i+0, true) / accScale,
        y: dataView.getInt16(i+2, true) / accScale,
        z: dataView.getInt16(i+4, true) / accScale,
      },
      gyr: {
        x: dataView.getInt16(i+6, true) / gyrScale,
        y: dataView.getInt16(i+8, true) / gyrScale,
        z: dataView.getInt16(i+10, true) / gyrScale,
      },
      mag: {
        x: dataView.getInt16(i+12, true) / magScale,
        y: dataView.getInt16(i+14, true) / magScale,
        z: dataView.getInt16(i+16, true) / magScale,
      },
    };

    movement.push(mov);
  }
  return {
    t: time,
    m: movement,
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
function reduceMovement(packet, delta) {
  const time = packet.t;
  const movement = packet.m;
  const n = movement.length;
  const result = [];
  for (let i = 0; i < n; i++) {
    const m = movement[i];
    const t = time - (n - i - 1) * delta;
    const value = { t, m };
    // console.log(value);
    result.push(value);
  }
  return result;
}
function movToLine(mov) {
  // console.log(mov);
  return `${mov.t}\t${mov.m.acc.x}\t${mov.m.acc.y}\t${mov.m.acc.z}\t${mov.m.gyr.x}\t${mov.m.gyr.y}\t${mov.m.gyr.z}\t${mov.m.mag.x}\t${mov.m.mag.y}\t${mov.m.mag.z}\n`;
}

function parseInterval(dataView) {
  return dataView.getUint16(0, true);
}

function parseSize(dataView) {
  return dataView.getUint16(0, true);
}

async function getEcgSamplingInterval(gattService) {
  const characteristic = await gattService.getCharacteristic(ecgMeasurementIntervalCharUUID16);
  const value = await characteristic.readValue();
  return parseInterval(value);
}
async function setEcgSamplingInterval(gattService, value) {
  const characteristic = await gattService.getCharacteristic(ecgMeasurementIntervalCharUUID16);
  const buffer = new ArrayBuffer(2);
  const dataView = new DataView(buffer);
  dataView.setUint16(0, value, true);
  await characteristic.writeValue(buffer);
  return await getEcgSamplingInterval(gattService);
}
async function getMovSamplingInterval(gattService) {
  const characteristic = await gattService.getCharacteristic(movMeasurementIntervalCharUUID16);
  const value = await characteristic.readValue();
  return parseInterval(value);
}
async function setMovSamplingInterval(gattService, value) {
  const characteristic = await gattService.getCharacteristic(movMeasurementIntervalCharUUID16);
  const buffer = new ArrayBuffer(2);
  const dataView = new DataView(buffer);
  dataView.setUint16(0, value, true);
  await characteristic.writeValue(buffer);
  return await getMovSamplingInterval(gattService);
}

// async function getPacketSize(gattService) {
//   const characteristic = await gattService.getCharacteristic(objectSizeCharUUID16);
//   const value = await characteristic.readValue();
//   return parseSize(value);
// }

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
    filters: [
    {namePrefix: 'Movesense'}],
    optionalServices: [
      ecgSvcUUID16,
      movSvcUUID16,
      activitySvcUUID16,
    ]
  });
  return await device.gatt.connect();
}

function onEcgData(evt) {
  const ecg = parseEcg(evt.target.value);
  const ecgReduced = reduceEcg(ecg, interval);
  console.log('ecg', ecg, evt.target.value, ecgReduced);
  for (const value of ecgReduced) {
    result += ecgToLine(value);
  }
}

function ecgToLine(value) {
  return `${value.t}\t${value.v}\n`;
}

var server;
var service;
var movChar;
var ecgChar;
var ecgInterval;
var movInterval;
var ecgListener;
var size;

var resultEcg = 't\tv\n';
var resultMov = 't\tacc_x\tacc_y\tacc_z\tgyr_x\tgyr_y\tgyr_z\tmag_x\tmag_y\tmag_z\n';

async function connect() {
  server = await connectToDevice();
  service = await server.getPrimaryService(activitySvcUUID16);
  console.log('connected');
}
async function subscribeEcg() {
  if (ecgChar) return;
  const interval = await getEcgSamplingInterval(service);
  document.getElementById('ecgInterval').value = interval;
  ecgInterval = interval;
  ecgChar = await service.getCharacteristic(ecgVoltageCharUUID16);
  ecgChar.addEventListener('characteristicvaluechanged', (evt) => {
    const dataView = evt.target.value;
    const dataPacket = parseEcg(dataView);
    const ecgData = reduceEcg(dataPacket, ecgInterval);
    for (let data of ecgData) {
        const line = ecgToLine(data);
        resultEcg += line;
    }
  });
  await ecgChar.startNotifications();
  console.log('ECG subscribed');
}
async function subscribeMov() {
  if (movChar) return;
  const interval = await getMovSamplingInterval(service);
  document.getElementById('movInterval').value = interval;
  movInterval = interval;
  movChar = await service.getCharacteristic(movCharUUID16);
  movChar.addEventListener('characteristicvaluechanged', (evt) => {
    const dataView = evt.target.value;
    const dataPacket = parseMovement(dataView);
    const movementData = reduceMovement(dataPacket, movInterval);
    for (let data of movementData) {
        const line = movToLine(data);
        resultMov += line;
    }
  });
  await movChar.startNotifications();
  console.log('Mov subscribed');
}

async function disconnect() {
  if (ecgListener) {
    stop();
  }
  await server.disconnect();
}

async function setEcgDelta() {
  const input = document.getElementById('ecgInterval');
  const value = parseInt(input.value);
  const setValue = await setEcgSamplingInterval(service, value);
  input.value = setValue.toString();
  console.log(`Set ecg interval to ${setValue}`);
}

async function setMovDelta() {
  const input = document.getElementById('movInterval');
  const value = parseInt(input.value);
  const setValue = await setMovSamplingInterval(service, value);
  input.value = setValue.toString();
  console.log(`Set mov interval to ${setValue}`);
}

function copyEcg() {
  navigator.clipboard.writeText(resultEcg);
}

function copyMov() {
  navigator.clipboard.writeText(resultMov);
}