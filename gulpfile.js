const gulp = require("gulp");
const exec = require('child_process').exec;

var buildCommand = "spcomp chillycomp.sp -i include -o dist/chillycomp";

gulp.task('build', () =>{
    exec(buildCommand, (error, stdout, stderr) => {
        console.log(stdout);
        console.log(stderr);
    });
});

gulp.task('watch', () => {
    gulp.watch('chillycomp.sp', ['build']);
    gulp.watch('include/**/*', ['build']);
});