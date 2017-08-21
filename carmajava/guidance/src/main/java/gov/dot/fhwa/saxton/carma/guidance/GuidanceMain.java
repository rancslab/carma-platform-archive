/*
 * TODO: Copyright (C) 2017 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

package gov.dot.fhwa.saxton.carma.guidance;

import java.util.ArrayList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import cav_msgs.BSM;
import cav_msgs.SystemAlert;
import gov.dot.fhwa.saxton.carma.guidance.pubsub.IPublicationChannel;
import gov.dot.fhwa.saxton.carma.guidance.pubsub.ISubscriptionChannel;
import gov.dot.fhwa.saxton.carma.guidance.pubsub.OnMessageCallback;
import org.apache.commons.logging.Log;
import org.ros.concurrent.CancellableLoop;
import org.ros.message.MessageListener;
import org.ros.namespace.GraphName;
import org.ros.node.AbstractNodeMain;
import org.ros.node.ConnectedNode;
import org.ros.node.parameter.ParameterTree;
import org.ros.node.topic.Publisher;
import org.ros.node.topic.Subscriber;

import gov.dot.fhwa.saxton.carma.guidance.pubsub.PubSubManager;

/**
 * The top-level Guidance package is responsible for providing basic facilities needed by all elements of
 * the Guidance package. It forms the Guidance ROS node.
 * <p>
 * Command line test: rosrun carmajava guidance gov.dot.fhwa.saxton.carmajava.guidance.GuidanceMain
 */
public class GuidanceMain extends AbstractNodeMain {

  @Override public GraphName getDefaultNodeName() {
    return GraphName.of("guidance_main");
  }

  /**
   * Initialize the runnable thread members of the Guidance package.
   */
  private void initExecutor(Log log) {
    executor = Executors.newFixedThreadPool(numThreads);
    Arbitrator arbitrator = new Arbitrator(pubSubManager);
    PluginManager pluginManager = new PluginManager(pubSubManager);
    TrajectoryExecutor trajectoryExecutor = new TrajectoryExecutor(pubSubManager);
    Tracking tracking = new Tracking(pubSubManager, log);

    executor.execute(arbitrator);
    executor.execute(pluginManager);
    executor.execute(trajectoryExecutor);
    executor.execute(tracking);
  }

  /**
   * Initialize the PubSubManager and setup it's message queue.
   */
  private void initPubSubManager(ConnectedNode node) {
    pubSubManager = new PubSubManager(node);
  }

  @Override public void onStart(final ConnectedNode connectedNode) {

    final Log log = connectedNode.getLog();

    // Currently setup to listen to it's own message. Change to listen to someone other topic.
    initPubSubManager(connectedNode);
    initExecutor(log);
    ISubscriptionChannel<SystemAlert> subscriber =
      pubSubManager.getSubscriptionChannelForTopic("system_alert", cav_msgs.SystemAlert._TYPE);

    subscriber.registerOnMessageCallback(new OnMessageCallback<SystemAlert>() {
      @Override public void onMessage(cav_msgs.SystemAlert message) {
        String messageTypeFullDescription = "NA";

        switch (message.getType()) {
          case cav_msgs.SystemAlert.CAUTION:
            messageTypeFullDescription = "Take caution! ";
            break;
          case cav_msgs.SystemAlert.WARNING:
            messageTypeFullDescription = "I have a warning! ";
            break;
          case cav_msgs.SystemAlert.FATAL:
            messageTypeFullDescription = "I am FATAL! ";
            break;
          case cav_msgs.SystemAlert.NOT_READY:
            messageTypeFullDescription = "I am NOT Ready! ";
            break;
          case cav_msgs.SystemAlert.SYSTEM_READY:
            messageTypeFullDescription = "I am Ready! ";
            break;
          default:
            messageTypeFullDescription = "I am NOT Ready! ";
        }

        log.info(
          "guidance_main heard: \"" + message.getDescription() + ";" + messageTypeFullDescription
            + "\"");

      }//onNewMessage
    }//MessageListener
    );//addMessageListener

    final IPublicationChannel<SystemAlert> systemAlertPublisher =
      pubSubManager.getPublicationChannelForTopic("system_alert", cav_msgs.SystemAlert._TYPE);

    //Getting the ros param called run_id.
    ParameterTree param = connectedNode.getParameterTree();
    final String rosRunID = param.getString("/run_id");
    //params.setString("~/param_name", param_value);

    // This CancellableLoop will be canceled automatically when the node shuts
    // down.
    connectedNode.executeCancellableLoop(new CancellableLoop() {
     private int sequenceNumber;

     @Override protected void setup() {
       sequenceNumber = 0;
     }//setup

     @Override protected void loop() throws InterruptedException {

       cav_msgs.SystemAlert systemAlertMsg = systemAlertPublisher.newMessage();
       systemAlertMsg.setDescription(
         "Hello World! " + "I am guidance_main. " + sequenceNumber + " run_id = " + rosRunID
           + ".");
       systemAlertMsg.setType(SystemAlert.CAUTION);
       systemAlertPublisher.publish(systemAlertMsg);
       sequenceNumber++;

       Thread.sleep(30000);
     }//loop

    }//CancellableLoop
    );//executeCancellableLoop
  }//onStart

  // Member Variables
  protected ExecutorService executor;
  protected int numThreads = 4;
  protected PubSubManager pubSubManager;
}//AbstractNodeMain
