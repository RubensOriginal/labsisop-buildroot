#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

cp $BASE_DIR/../status-server/TcpServer $BASE_DIR/target/usr/bin
chmod +x $BASE_DIR/target/usr/bin/TcpServer