import {testAmount1_18, testRawTx, testTxWithAmount} from "./tools";


// Test from replayed transaction:
// https://etherscan.io/getRawTx?tx=0xf8bd307e00342b3c541b6d24c1bd42ab4fa0c383e3d96fcd77a3a84d8270144b
const rawTx = "0x02f891012b85056c65a46c85056c65a46c83050cda94ab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c80a4b6b55f250000000000000000000000000000000000000000000000015af1d78b58c40000c080a08c686c858e7ebda3eacac02dc4c01a0b635e91e53d8c5053f485e680b6b57e75a019418c235997167b828b31c192c0b3312c4eb83c6771fc127580b0ddae954eed";
testRawTx('deposit', rawTx, 2); // max fees splits to 3 screens

// Test constructed tx
const abi = require('../harvest/abis/harvest_vault.json');
const contractAddr = "0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c";
testTxWithAmount(contractAddr, abi, 'deposit', testAmount1_18);
