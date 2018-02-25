# interoberlin-mynewt-extensions

Testbed for  mynewt-repository.
Please contribute only in the branch develop (at least for now).

## Setting it up

In your project directory (`newt new myproject`) edit the `project.yml`:

```
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
# 
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

project.name: "Baumhaus-Firmware"

project.repositories:
    - apache-mynewt-core
    - interoberlin-mynewt-extensions

# Use github's distribution mechanism for core ASF libraries.
# This provides mirroring automatically for us.
#
repository.apache-mynewt-core:
    type: github
    vers: 1-latest
    user: interoberlin
    repo: mynewt-core

repository.interoberlin-mynewt-extensions:
    type: github
    vers: 0-latest
    user: interoberlin
    repo: interoberlin-mynewt-extensions

```
Then issue a `newt install` or `newt update`. You could also go into the `repos/` repository-directory and work directly with `git`.

## Hardware / Board Support Packages

This repository contains a BSP for a [nrf51dk](https://github.com/interoberlin/interoberlin-mynewt-extensions/tree/master/hw/bsp/nrf51dk-16kbram-openocd) with 16kb RAM and changes to load and debug via OpenOCD. It is basically a fork from the original [nrf51dk-16kbram](https://github.com/apache/mynewt-core/tree/master/hw/bsp/nrf51dk-16kbram) and tries to be as minimal as possible. I.e. it back links to the original BSP wherever it's possible.
