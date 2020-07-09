git fetch
git pull
pm2 stop 'bedroom-lights-api'
pm2 delete 'bedroom-lights-api'
pm2 start 'node server.js' --name 'bedroom-lights-api'