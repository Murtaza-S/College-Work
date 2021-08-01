import tensorflow.compat.v1 as tf
import os
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import numpy as np
tf.disable_v2_behavior()

#Hyperparameters and other parameters

neural_layers = 3 
neurons_no = 100 
n_outputs = 10 
max_epoch = 50
batch_size = 150
train_keep_prob = 0.8
epoch_no_update = 0         
epochs_limit = 3  #Limit for epochs with no updation in loss
row_no = 28 # Lengh of each row
time_steps = 28 # Number of time steps

# Generating the Batches
def batches(train_data,train_labels): 
    random_id = np.random.permutation(len(train_data))
    n_batches = len(train_data) // batch_size
    for batch_idx in np.array_split(random_id, n_batches):
        x_batch = train_data[batch_idx]
        y_batch = train_labels[batch_idx]
        yield x_batch, y_batch

#Dataframe Casting 
def casting(x_train_va, x_train, y_train_va, y_train, x_test, y_test):

    x_train = x_train.astype(np.float32).reshape(-1, row_no, time_steps)/ 255.0  
    x_test = x_test.astype(np.float32).reshape(-1, row_no, time_steps)/ 255.0  
    x_train_va=  x_train_va.astype(np.float32).reshape(-1, row_no, time_steps)
    
    y_train = y_train.astype(np.int32)
    y_test = y_test.astype(np.int32)
    y_train_va = y_train_va.astype(np.int32)

    return x_train_va, x_train, y_train_va, y_train, x_test, y_test

def plot_graphs(valid_accuracy, valid_loss, epoch):

    x1 = [i+1 for i in range(len(valid_accuracy))]
    x2 = [i+1 for i in range(len(valid_loss))]

    plt.subplot(1, 2, 1)
    plt.plot(x1, valid_accuracy)
    plt.xlabel("Epochs")
    plt.ylabel('Validation Accuracy')

    plt.subplot(1, 2, 2)
    plt.plot(x2, valid_loss)
    plt.xlabel("Epochs")
    plt.ylabel('Validation Losses')

    plt.show()
    


# Main Function

if __name__ == "__main__" :   

    # Dataset 
    (x_train, y_train), (x_test, y_test) = tf.keras.datasets.fashion_mnist.load_data() 

    # Splitting
    x_train_va, x_train, y_train_va, y_train = train_test_split(x_train,y_train,test_size =0.083, random_state = 42)

    #Casting 
    x_train_va, x_train, y_train_va, y_train, x_test, y_test = casting(x_train_va, x_train, y_train_va, y_train, x_test, y_test)


    

    with tf.name_scope("Inputs"):
        X = tf.placeholder(tf.float32, [None, time_steps, row_no], name="X")
        y = tf.placeholder(tf.int32, [None], name="y")
        keep_prob = tf.placeholder_with_default(1.0, shape=(), name='keep_probability')


    input = tf.unstack(X, time_steps, 1)

    lstm_drop_cells = [tf.nn.rnn_cell.DropoutWrapper(cell, input_keep_prob=keep_prob)
    for cell in [tf.nn.rnn_cell.BasicRNNCell(num_units = neurons_no, activation = tf.nn.relu)
        for layer in range(neural_layers)]]

    multi_cell = tf.nn.rnn_cell.MultiRNNCell(lstm_drop_cells)

    outputs, states = tf.nn.static_rnn(multi_cell, input, dtype = tf.float32) 
        
        
    with tf.name_scope("Loss"):
        states_concat = tf.concat(axis=1, values=states, name='states_reshape')
        layer_1 = tf.layers.dense(states_concat, 64, name='Layer_1')
        layer_2 = tf.layers.dense(layer_1, 32, name='Layer_2')
        logits = tf.layers.dense(layer_2, n_outputs, name='output_layer')
        entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=y, logits=tf.reshape(logits, shape=(-1, n_outputs)), name='softmax_cross_entropy')
        loss = tf.reduce_mean(entropy, name='loss')
        loss_summary = tf.summary.scalar('loss_summ', loss)

    with tf.name_scope("Train"):    
        optimizer = tf.train.AdamOptimizer(learning_rate=0.001, name='Adam_optimizer')
        training_optimizer = optimizer.minimize(loss, name='training_Adam')

    with tf.name_scope("Evaluation"):        
        correct = tf.nn.in_top_k(tf.reshape(logits, (-1, n_outputs)), y, 1, name='inTopK')
        accuracy = tf.reduce_mean(tf.cast(correct, tf.float32), name='Accuracy')
        accuracy_summary = tf.summary.scalar('Accuracy_Summ', accuracy)



    best_loss = np.infty               
     
    valid_accuracy = []
    valid_loss = []
    epoch = 0

    saver = tf.train.Saver()
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        epoch = 0

        print("Epoch no".center(10), "Validation Accurecy".center(25), "Loss".center(10))

        while epoch < max_epoch:

            #Yield becomes generator for the loop
            for x_batch, y_batch in batches(x_train, y_train):
                sess.run(training_optimizer, feed_dict={X: x_batch, y: y_batch, keep_prob: train_keep_prob})
            
            valid_a, val_loss, acc_sum, loss_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_train, y: y_train, keep_prob: train_keep_prob})   
            
            valid_accuracy.append(valid_a*100)
            valid_loss.append(val_loss)
            
            if val_loss >= best_loss:
                epoch_no_update += 1
                if epoch_no_update == epochs_limit:
                    print('Early Stopping')
                    break
                
            else:
                best_loss = val_loss
                best_epoch_no = epoch
                epoch_no_update = 0
                #Saving the Best weights for the retrival.
                best_sess_run = saver.save(sess, "./temp/alpha.ckpt")

            # print(str(epoch).center(10),
            #     f" {valid_a * 100:.3f}%".center(25), f'{val_loss:.3f}'.center(10))
                
            epoch += 1
            if epoch % 3:
                continue

            print(str(epoch).center(10),
                f" {valid_a * 100:.3f}%".center(25), f'{val_loss:.3f}'.center(10))
            

        # Restoring the saved path and Running the test data-set through

        save_path = saver.restore(sess, "./temp/alpha.ckpt")
        test_accuracy, test_loss, test_a_sum, test_l_sum = sess.run([accuracy, loss, accuracy_summary, loss_summary], feed_dict={X: x_test, y: y_test})

        # Printing the accuracy of Test Dataset
        print("\n\nTest dataset Accurecy : ", f" {test_accuracy * 100:.3f}%")


    plot_graphs(valid_accuracy, valid_loss, epoch)