git fetch
git pull
pm2 stop 'BedroomLightsAPI'
pm2 start 'node server.js' --name 'BedroomLightsAPI'