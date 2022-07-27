import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models, txFromEtherscan} from './test.fixture';
import { ethers } from "ethers";
import { parseEther, parseUnits} from "ethers/lib/utils";

export const testAmount1_18 = parseUnits("1.2345", '18');

export async function testRawTx(method, rawTx) {
  nano_models.forEach(function (model) {
    test('[Nano ' + model.letter + '] '+method+' raw_tx', zemu(model, async (sim, eth) => {

      const serializedTx = txFromEtherscan(rawTx);

      const tx = eth.signTransaction(
        "44'/60'/0'/0",
        serializedTx,
      );

      const right_clicks = model.letter === 'S' ? 12 : 5;

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);
      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      await sim.navigateAndCompareSnapshots('.', model.name + '_'+method+'_raw_tx', [right_clicks, 0]);

      await tx;
    }));
  });
}

// Test from constructed transaction
export async function testTxWithAmount(contractAddr, abi, method, amount) {

  nano_models.forEach(function (model) {
    test('[Nano ' + model.letter + '] '+method, zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      // Constants used to create the transaction
      // const amount = parseUnits("1.2345", '18');

      const {data} = await contract.populateTransaction[method](amount);

      // Get the generic transaction template
      let unsignedTx = genericTx;
      // Modify `to` to make it interact with the contract
      unsignedTx.to = contractAddr;
      // Modify the attached data
      unsignedTx.data = data;
      // Modify the number of ETH sent
      unsignedTx.value = parseEther("0");

      // Create serializedTx and remove the "0x" prefix
      const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);

      const tx = eth.signTransaction(
        "44'/60'/0'/0",
        serializedTx
      );

      const right_clicks = model.letter === 'S' ? 7 : 5;

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);
      // Navigate the display by pressing the right button N times, then pressing both buttons to accept the transaction.
      await sim.navigateAndCompareSnapshots('.', model.name + '_'+method, [right_clicks, 0]);

      await tx;
    }));
  });
}
