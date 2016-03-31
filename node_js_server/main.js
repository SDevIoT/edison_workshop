var http = require('http'),
        fs = require('fs');
var serverPort = 8124;

fs.readFile('./main.html', function (err, html) {
    if (err) {
        throw err;
    }
    http.createServer(function (request, response) {
        
        response.writeHeader(200, {"Content-Type": "text/html"});
        response.write(html);
        response.end();
    }).listen(serverPort);

    http.createServer(function (request, response) {
        response.writeHeader(200, {
            "Content-Type": "text/html",
            "Access-Control-Allow-Origin":"*"
        });
        var datetime = new Date();
        response.write("Command Received!  (" + datetime + ")");
        response.end();

        fs.writeFile('command.txt', 'MOVE', function (err) {
            if (err)
                throw err;
            console.log('File saved!');
        });


    }).listen(8125);


    console.log("Listening to port " + serverPort);
    console.log("Listening to port " + 8125);
});