var mysql = require('mysql');

var db = mysql.createConnection({
	  host     : 'db-leafeon.cp1bodcsjnqi.us-west-1.rds.amazonaws.com',
	  user     : 'wkdaslwns0427',
	  password : 'pawpaw0426',
	  database : 'leafeon_db'
	})
db.connect();
module.exports = db