var express = require('express');
var bodyParser = require('body-parser');
var fs = require('fs');

var app = express();
const DB_FILE_PATH = './lightsState.json';

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use(function (req, res, next) {
  res.header("Access-Control-Allow-Origin", "*"); // update to match the domain you will make the request from
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});

function getDbFile() {
  let fileData = fs.readFileSync(DB_FILE_PATH).toString();
  let file = JSON.parse(fileData);
  return file;
}

function updateDbFile(data) {
  let fileExists = fs.existsSync(DB_FILE_PATH);

  if (!fileExists) {
    fs.writeFileSync(DB_FILE_PATH, JSON.stringify({
      state: data.state || 'off',
      turnOnTime: data.turnOnTime || null,
      daysOfWeek: data.daysOfWeek || ['SEG', 'TER', 'QUA', 'QUI', 'SEX', 'SAB', 'DOM']
    }))
  }
  else {
    let file = getDbFile();

    file.state = data.state || file.state;
    file.turnOnTime = data.turnOnTime || file.turnOnTime;
    file.daysOfWeek = data.daysOfWeek || file.daysOfWeek;

    fs.writeFileSync(DB_FILE_PATH, JSON.stringify(file))
  }
}

app.post('/lights', function (req, res) {
  const { state, daysOfWeek, turnOnTime } = req.body;

  try {
    updateDbFile({
      state: state,
      daysOfWeek: daysOfWeek,
      turnOnTime: turnOnTime
    })
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }

  return res.status(200);
});

app.post('/lights/toggle', function (req, res) {
  try {
    let oldState = getDbFile().state;

    switch (oldState) {
      case 'off':
        updateDbFile({ state: 'full' })
        break;
      case 'half':
      case 'full':
        updateDbFile({ state: 'off' })
        break;
    }
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }

  return res.status(200);
});

app.get('/lights', function (req, res) {
  const { daysOfWeek, time } = req.body;

  try {
    let file = getDbFile();
    return res.status(200)
      .json(file)
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});