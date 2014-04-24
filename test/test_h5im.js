var assert = require("assert");
var should = require("should");

var hdf5 = require('hdf5');
var h5im = require('h5im');

var Access = {
  ACC_RDONLY :	0,	/*absence of rdwr => rd-only */
  ACC_RDWR :	1,	/*open for read and write    */
  ACC_TRUNC :	2,	/*overwrite existing files   */
  ACC_EXCL :	3,	/*fail if file already exists*/
  ACC_DEBUG :	4,	/*print debug info	     */
  ACC_CREAT :	5	/*create non-existing files  */
};

describe("testing images ",function(){

    describe("read image and transfer",function(){
        // open hdf file
        var file;
        var file2;
        var res;
        var image;
        before(function(){
          file = new hdf5.File('./h5im.h5', Access.ACC_TRUNC);
          file2 = new hdf5.File('./test/examples/hdf5.h5', Access.ACC_RDONLY);
        });
        var group;
        it("should be -1 yet", function(){
            group=file.createGroup();
            group.id.should.equal(-1);
        });
        it("should be >0 ", function(){
            group.create('pmc', file);
            group.id.should.not.equal(-1);
        });
        it("should be an image ", function(){
            res=h5im.isImage(file2.id, 'hdf_logo.jpg');
            res.should.equal(true);
        });
        it("image width should be 48 ", function(){
            image=h5im.readImage(file2.id, 'hdf_logo.jpg');
            image.width.should.equal(48);
        });
        it("image height should be 45 ", function(){
            image.height.should.equal(45);
        });
        it("image interlace should be INTERLACE_PIXEL ", function(){
            image.interlace.should.equal('INTERLACE_PIXEL');
        });
        it("image planes should be 3 ", function(){
            image.planes.should.equal(3);
        });
        it("image length should be 6480 ", function(){
            image.length.should.equal(6480);
        });
        it("make image  ", function(){
            h5im.makeImage(group.id, 'hdf_logo.jpg', image);
        });



    });
});
