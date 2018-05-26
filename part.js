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
                    name: 'lakeMaryMosquito'
                });
                l(lakeMaryMosquito);
                var fnPr = particle.callFunction({
                    deviceId: lakeMaryMosquito.id,
                    name: 'toggleBlue',
                    argument: 'on',
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
