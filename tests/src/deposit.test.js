import {testAmount1_18, testRawTx, testTxWithAmount} from "./tools";

// Test from replayed transaction:
// https://etherscan.io/tx/0xae83e516d844cd16688c008700d9d094bad1d8413163f665def9f16518ad1c1b
const rawTx = "0x02f8920181908501e93616b48501e93616b483050c2b94ab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c80a4b6b55f25000000000000000000000000000000000000000000001fc3842bd1f071c00000c080a0f324058765a4a07ee3d17cf66da4d01165d391e0f7d9b0270e6754f865adffc6a05dbd0ccc3bd76f87165037b488d6d919b0fedb46a811e12a9d09f78e00a62815";
testRawTx('deposit', rawTx).then();

// Test constructed tx
const abi = require('../harvest/abis/harvest_vault.json');
const contractAddr = "0xab7fa2b2985bccfc13c6d86b1d5a17486ab1e04c";
testTxWithAmount(contractAddr, abi, 'deposit', testAmount1_18).then();
