// Run this script to update Harvest vaults and pools to contracts-info.txt and b2c.json

import fetch from 'node-fetch';
import fs from 'fs';
import {utils} from 'ethers';

const VAULTS_URL = 'https://api-ui.harvest.finance/vaults?key=41e90ced-d559-4433-b390-af424fdc76d6'

const b2cFile = 'harvest/b2c.json';
const contractsInfoFile = '../src/contracts-info.txt';

function b2cTemplate(chainId, contracts) {
  return {
    "chainId": chainId,
    "contracts": contracts,
    "name": "Harvest"
  }
}

function b2cVaultTemplate(vault) {
  // address must be lowercase
  return {
    "address": vault.vaultAddress.toLowerCase(),
    "contractName": "Vault",
    "selectors": {
      "0xb6b55f25": {"erc20OfInterest": [],"method": "deposit","plugin": "Harvest"},
      "0x2e1a7d4d": {"erc20OfInterest": [],"method": "withdraw","plugin": "Harvest"}
    }
  }
}

function b2cPoolTemplate(vault) {
  // address must be lowercase
  return {
    "address": vault.rewardPool.toLowerCase(),
    "contractName": "NoMintRewardPool",
    "selectors": {
      "0xa694fc3a": {"erc20OfInterest": [],"method": "stake","plugin": "Harvest"},
      "0x3d18b912": {"erc20OfInterest": [],"method": "getReward","plugin": "Harvest"},
      "0xe9fad8ee": {"erc20OfInterest": [],"method": "exit","plugin": "Harvest"}
    }
  }
}

function contractsInfoVaultTemplate(v) {
  // address must be mixed case checksum
  return `_HARVEST("${utils.getAddress(v.vaultAddress)}", "${v.id} ", ${v.decimals}, "f${v.id} ", 18);`;
}

function contractsInfoPoolTemplate(v) {
  // address must be mixed case checksum
  return `_HARVEST("${utils.getAddress(v.rewardPool)}", "f${v.id} ", 18, "", 18);`;
}

async function fetchJson(url) {
  const response = await fetch(url);
  return response.json();
}

async function updateContractsInfo(vaultsUrl) {
  const allVaults = await fetchJson(vaultsUrl);

  const vaults = allVaults.eth;
  // console.log('vaults', vaults);

  let contractsInfo = [];
  const contracts = [];

  for (const id in vaults) {
    const vault = vaults[id];

    if (vault.inactive) continue;

    contracts.push(b2cVaultTemplate(vault));
    // if vault have rewardPool then add its contract
    contractsInfo.push(contractsInfoVaultTemplate(vault));
    if (vault.rewardPool) {
      contracts.push(b2cPoolTemplate(vault));
      contractsInfo.push(contractsInfoPoolTemplate(vault));
    }
  }

  const callback = (err) => {
    if (err)
      console.error(err);
    else
      console.log('SUCCESS :)')
  };

  const chainId = 1; // ETH MAINNET
  const b2c = b2cTemplate(chainId, contracts);
  console.log('b2c', b2c);
  fs.writeFile(b2cFile, JSON.stringify(b2c, undefined, '\t'), callback);

  const ci = contractsInfo.join('\n');
  console.log('ci\n', ci);
  fs.writeFile(contractsInfoFile, ci, callback);

}

updateContractsInfo(VAULTS_URL).then();
