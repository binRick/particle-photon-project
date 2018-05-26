#!/usr/bin/env node

var Particle = require('particle-api-js'),
    particle = new Particle(),
    _ = require('underscore'),
    c = require('chalk'),
    async = require('async'),
    pj = require('prettyjson'),
    request = require('request'),
    l = console.log,
    config = require('./config.js'),
    token;


var state = process.argv[2];
if (state != 'on' && state != 'off') {
    l(c.red('First argument must be "on" or "off"'));
    process.exit(1);
}

particle.login({
    username: config.login.user,
    password: config.login.pass,
}).then(
    function(data) {
        token = data.body.access_token;
        l('logged in', data.body);
        var devicesPr = particle.listDevices({
            auth: token
        });
        devicesPr.then(
            function(devices) {
                var lakeMaryMosquito = _.findWhere(devices.body, {
                    name: config.deviceName,
                });
                l(lakeMaryMosquito);
                var fnPr = particle.callFunction({
                    deviceId: lakeMaryMosquito.id,
                    name: 'toggleBlue',
                    argument: state,
                    auth: token
                });

                fnPr.then(
                    function(data) {
                        console.log('Function called succesfully:', data);
                    },
                    function(err) {
                        console.log('An error occurred:', err);
                    });

            },
            function(err) {
                console.log('List devices call failed: ', err);
            }
        );


    },
    function(err) {
        console.log('Could not log in.', err);
    }
);
