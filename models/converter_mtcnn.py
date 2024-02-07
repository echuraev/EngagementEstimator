import tvm
import tvm.testing
from tvm import relay
from tvm.contrib import utils, xcode, coreml_runtime, graph_runtime

from tvm.relay import transform
from tvm.relay.build_module import bind_params_by_name

import tensorflow as tf

target = "llvm -mtriple=x86_64-apple-darwin20.1.0"
model_name = "./performance/tf-mtcnn/mtcnn"
model = model_name + ".pb"

def open_tf_model(path):
    def load_graph_def(frozen_graph_filename):
        graph_def = None
        with tf.io.gfile.GFile(frozen_graph_filename, 'rb') as f:
            graph_def = tf.compat.v1.GraphDef()
            graph_def.ParseFromString(f.read())
        return graph_def

    def load_mtcnn(sess, graph):
        #print("Graph:")
        #print(graph.as_graph_def())
        pnet_out_1 = graph.get_tensor_by_name('pnet/conv4-2/BiasAdd:0')
        pnet_out_2 = graph.get_tensor_by_name('pnet/prob1:0')
        pnet_in = graph.get_tensor_by_name('pnet/input:0')

        rnet_out_1 = graph.get_tensor_by_name('rnet/conv5-2/conv5-2:0')
        rnet_out_2 = graph.get_tensor_by_name('rnet/prob1:0')
        rnet_in = graph.get_tensor_by_name('rnet/input:0')

        onet_out_1 = graph.get_tensor_by_name('onet/conv6-2/conv6-2:0')
        onet_out_2 = graph.get_tensor_by_name('onet/conv6-3/conv6-3:0')
        onet_out_3 = graph.get_tensor_by_name('onet/prob1:0')
        onet_in = graph.get_tensor_by_name('onet/input:0')

        pnet_fun = lambda img : sess.run((pnet_out_1, pnet_out_2), feed_dict={pnet_in:img})
        rnet_fun = lambda img : sess.run((rnet_out_1, rnet_out_2), feed_dict={rnet_in:img})
        onet_fun = lambda img : sess.run((onet_out_1, onet_out_2, onet_out_3), feed_dict={onet_in:img})
        return pnet_fun, rnet_fun, onet_fun

    import tensorflow as tf

    from tensorflow.compat.v1.keras.backend import set_session

    config = tf.compat.v1.ConfigProto()
    config.gpu_options.allow_growth = True
    sess=tf.compat.v1.Session(config=config)
    set_session(sess)

    with tf.Graph().as_default() as full_graph:
        gd = load_graph_def(path)
        layout = "NHWC"
        N = tvm.tir.Any()
        N = 1
        H = 1024
        W = 1024
        shape_dict = {
            "input": (H, W, 3),
            "min_size": (1,),
            "threshold": (3,),
            "factor": (1,),
        }
        mod, params = relay.frontend.from_tensorflow(gd, layout=layout, shape=shape_dict)

        print(mod)
        exit(123)
        tf.import_graph_def(gd, name="mtcnn")
        print([n.name for n in gd.node])
    sess = tf.compat.v1.Session(graph=full_graph)
    pnet, rnet, onet = load_mtcnn(sess, full_graph)


#def open_tf_model(path):
#    import tensorflow as tf
#
#    print(tf.__version__)
#    from tensorflow.compat.v1.keras.backend import set_session 
#    config = tf.compat.v1.ConfigProto()
#    config.gpu_options.allow_growth = True
#    sess=tf.compat.v1.Session(config=config)
#    set_session(sess)
#
#    from facial_analysis import FacialImageProcessing
#    imgProcessing=FacialImageProcessing(False)
#    print(imgProcessing.pnet)
#    print(imgProcessing.rnet)
#    print(imgProcessing.onet)

if __name__ == '__main__':
    lib = open_tf_model(model)
    #lib_name = model_name + ".so"
    #path_dso = lib_name
    #lib.export_library(path_dso)




